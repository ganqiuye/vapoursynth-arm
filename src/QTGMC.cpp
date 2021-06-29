/*================================================================
*
*   File:		QTGMC.cpp
*   Author:		Gan Qiuye(ganqiuye@163.com)
*   Date:		2021-05-12  13:37
*   Version:		1.0
*   Description:
*
================================================================*/

#include "QTGMC.h"
#include <cmath>
#include <algorithm>
#include <iostream>

#define MAX(x, y)  ((x) > (y) ? (x) : (y))
#define MIN(x, y)  ((x) < (y) ? (x) : (y))

template<typename ... Args>
static std::string str_format(const std::string &format, Args ... args)
{
    auto size_buf = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; 
    std::unique_ptr<char[]> buf(new(std::nothrow) char[size_buf]);

    if (!buf)
        return std::string("");

    std::snprintf(buf.get(), size_buf, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size_buf - 1); 
}

QTGMC::QTGMC(const VSAPI* vsapi, VSCore *core, VSNodeRef* node, QtgmsArgs* args)
{
    //Todo
}


MiniQTGMC::MiniQTGMC(const VSAPI* vsapi, VSCore *core, VSNodeRef* node)
{
    this->vsapi = vsapi;
    this->core = core;
    input = node;
    mArgs.Rep0         = 4;
    mArgs.EdiMode      = QtgmsArgs::NNEDI3;
    mArgs.NNSize       = 1;
    mArgs.NNeurons     = 1;
    mArgs.EdiMaxD      = 7;
    mArgs.BlockSize    = 16;
    mArgs.Overlap      = 8;
    mArgs.Search       = 5;
    mArgs.SearchParam  = 2;
    mArgs.PelSearch    = 2;
    mArgs.ChromaMotion = true;

    const VSVideoInfo *vi = vsapi->getVideoInfo(node);
    bitsPerSample = vi->format->bitsPerSample;
    neutral = 1 << (bitsPerSample - 1);
    peak = (1 << bitsPerSample) - 1;
    isGray = false;

    w = vi->width;
    h = vi->height;
    epsilon = 0.0001;

    hpad = w - (floor(((w - mArgs.Overlap) / (mArgs.BlockSize - mArgs.Overlap))) * (mArgs.BlockSize - mArgs.Overlap) + mArgs.Overlap);
    vpad = h - (floor((h - mArgs.Overlap) / (mArgs.BlockSize - mArgs.Overlap)) * (mArgs.BlockSize - mArgs.Overlap) + mArgs.Overlap);
    hpad = MAX(hpad, 8); // But match default padding if possible
    vpad = MAX(vpad, 8);
    nnedi3 = new Nnedi3Wrapper(vsapi, core);
    std    = new StdWrapper(vsapi, core);
    fmtc   = new FmtcWrapper(vsapi, core);
    misc   = new MiscWrapper(vsapi, core);
    mv     = new MvtoolsWrapper(vsapi, core);
    rgvs   = new RemoveGrainWrapper(vsapi, core);
    text   = new TextWrapper(vsapi, core);
}

MiniQTGMC::~MiniQTGMC()
{
    if(!nnedi3)
        delete nnedi3;
    if(!std)
        delete std;
    if(!fmtc)
        delete fmtc;
    if(!misc)
        delete misc;
    if(!mv)
        delete mv;
    if(!rgvs)
        delete rgvs;
    if(!text)
        delete text;
}

static inline int scale(int value, int peak)
{
    return floor(value * peak / 255);
}

VSNodeRef* MiniQTGMC::Bob(VSNodeRef* clip, double b, double c, bool tff)
{
    const VSVideoInfo *vi = vsapi->getVideoInfo(clip);
    int bits = vi->format->bitsPerSample;
    VSNodeRef *out;
    VSNodeRef *node = std->separateFields(clip, tff);

    ResampleData data;
    fmtc->resample_data_init(&data);
    data.scalev = 2.0;
    data.kernel = "bicubic";
    data.a1.push_back(b);
    data.a1.push_back(c);
    data.interlaced  = 1;
    data.interlacedd = 0;
    VSNodeRef *r_node = fmtc->resample(node, &data);
    out = r_node;

    vi = vsapi->getVideoInfo(r_node);
    if(bits != vi->format->bitsPerSample)
    {
        BitdepthData bitData;
        fmtc->bitdepth_data_init(&bitData);
        bitData.bits = bits;
        bitData.dmode = 1;
        VSNodeRef *b_node = fmtc->bitdepth(r_node, &bitData);
        out = b_node;
    }
    vsapi->freeNode(node);
    return out;
}

VSNodeRef* MiniQTGMC::SCDetect(VSNodeRef* clip, double threshold)
{
    //if rgb
    VSNodeRef* sc = misc->SCDetect(clip, threshold);
    //if rgb
    return sc;
}

VSNodeRef* MiniQTGMC::QTGMC_KeepOnlyBobShimmerFixes(VSNodeRef* input, VSNodeRef* Ref, int Rep, bool Chroma)
{
    const VSVideoInfo *vi = vsapi->getVideoInfo(input);
    int neutral = 1 << (vi->format->bitsPerSample - 1);
    int peak = (1 << vi->format->bitsPerSample) - 1;

    bool isGray = (vi->format->colorFamily == cmGray);//false
    std::vector<int> planes;
    if(Chroma && !isGray)
    {
        planes.push_back(0);
        planes.push_back(1);
        planes.push_back(2);
    }
    else
    {
        planes.push_back(0);
    }

    int ed = Rep<10 ? Rep:floor(Rep / 10);
    int od = Rep<10 ? 0 : Rep%10;
    std::vector<int> tmp;
    VSNodeRef* diff = std->makeDiff(Ref, input, tmp);   
   // Areas of positive difference                                 ed = 0 1 2 3 4 5 6 7
 
    std::vector<int> coordinates{0, 1, 0, 0, 0, 0, 1, 0};
    VSNodeRef* choke1 = std->minimum(diff, planes, coordinates); //      x x x x x x x x    1 pixel   

    if(ed > 2)
    {
        choke1 = std->minimum(choke1, planes, coordinates);   //      . . . x x x x x    1 pixel      Deflate to remove thin areas
    }
    if(ed > 5)
    {
        choke1 = std->minimum(choke1, planes, coordinates);  //      . . . . . . x x    1 pixel  
    }
    if(ed % 3 != 0)
    {
        choke1 = std->deflate(choke1, planes);               //      . x x . x x . x    A bit more deflate & some horizonal effect
    }
    if(ed >= 2 && ed <= 5) 
    {
        choke1 = std->median(choke1, planes);               //      . . x . . x . .    Local median
    }
    choke1 = std->maximum(choke1, planes, coordinates);     //      x x x x x x x x    1 pixel  
    if(ed > 1)
    {
        choke1 = std->maximum(choke1, planes, coordinates); //      . . x x x x x x    1 pixel    Reflate again
    }
    if(ed > 4)
    {
        choke1 = std->maximum(choke1, planes, coordinates); //     . . . . . x x x     1 pixel 
    } 

    // Over-dilation - extra reflation up to about 1 pixel
    if(od == 1)
    {
        choke1 = std->inflate(choke1, planes);
    }
    else if(od == 2)
    {
        choke1 = std->inflate(choke1, planes);
        choke1 = std->inflate(choke1, planes);
    }
    else if(od >= 3)
    {
        std::vector<int> tmpCoordinates;
        choke1 = std->maximum(choke1, planes, tmpCoordinates);
    }

    //Areas of negative difference (similar to above)
    VSNodeRef* choke2 = std->maximum(diff, planes, coordinates);
    if(ed > 2)
    {
        choke2 = std->maximum(choke2, planes, coordinates);
    }
    if(ed > 5)
    {
        choke2 = std->maximum(choke2, planes, coordinates);
    }
    if(ed % 3 != 0)
    {
        choke2 = std->inflate(choke2, planes);
    }
    if(ed >= 2 && ed <= 5)
    {
        choke2 = std->median(choke2, planes);
    }
    choke2 = std->minimum(choke2, planes, coordinates);
    if(ed > 1)
    {
        choke2 = std->minimum(choke2, planes, coordinates);
    }
    if(ed > 4)
    {
        choke2 = std->minimum(choke2, planes, coordinates);
    }
    if(od == 1)
    {
        choke2 = std->deflate(choke2, planes);
    }
    else if(od == 2)
    {
        choke2 = std->deflate(choke2, planes);
        choke2 = std->deflate(choke2, planes);
    }
    else if(od >= 3)
    {
        std::vector<int> tmpCoordinates;
        choke2 = std->minimum(choke2, planes, tmpCoordinates);
    }

    //Combine above areas to find those areas of difference to restore
    std::string expr1 = str_format("x %d < x y %d < %d y ? ?", scale(129, peak),neutral, neutral);
    std::string expr2 = str_format("x %d < x y %d < %d y ? ?", scale(127, peak),neutral, neutral);
    std::vector<VSNodeRef*> exprNodes1{diff, choke1};
    std::vector<const std::string> exprs1{expr1};
    if(!(Chroma || isGray))
        exprs1.push_back("");

    VSNodeRef* tmpExprNode = std->expr(exprNodes1, exprs1);

    std::vector<VSNodeRef*> exprNodes2{tmpExprNode, choke2};
    std::vector<const std::string> exprs2{expr2};
    if(!(Chroma || isGray))
        exprs2.push_back("");

    VSNodeRef* restore = std->expr(exprNodes2, exprs2);
    VSNodeRef* out = std->mergeDiff(input, restore, planes);
    vsapi->freeNode(diff);
    vsapi->freeNode(choke1);
    vsapi->freeNode(choke2);
    vsapi->freeNode(tmpExprNode);
    vsapi->freeNode(restore);
    return out;
}

VSNodeRef* MiniQTGMC::Resize(VSNodeRef* src, int w, int h, 
                      std::vector<double> sx, std::vector<double> sy, 
                      std::vector<double> sw, std::vector<double> sh, 
                      std::string kernel,
                      std::vector<double> a1, std::vector<double> a2,
                      int dmode,
                      std::vector<int64_t> invks,  std::vector<int64_t> invkstaps,
                      std::vector<int> planes, std::vector<int64_t> taps, 
                      bool noring, std::string css, std::vector<int64_t> center, 
                      std::string cplace, std::string cplaces, std::string cplaced,
                      int interlaced, int interlacedd, int tff, int tffd, int flt,
                      int bits, int fulls, int fulld, int ampo, int ampn, 
                      int dyn, int staticnoise, int patsize)
{
    const VSVideoInfo *vi = vsapi->getVideoInfo(src);

    if(bits == NONE)
        bits = vi->format->bitsPerSample;//todo

    int sr_h = w / vi->width;
    int sr_v = h / vi->height;
    int sr_up = MAX(sr_h, sr_v);
    int sr_dw = 1 / MIN(sr_h, sr_v);
    int sr = MAX(sr_up, sr_dw);
    assert(sr >= 1);


    float thr = 2.5;
    bool nrb = sr > thr;
    bool nrf = (sr < (thr + 1)) && noring;
    int nrv;
    std::vector<float> nrvs;
    if(nrb)
    {
        int nrr = MIN(sr - thr, 1);
        nrv = floor((1 - nrr) * 255 + 0.5);
        nrv = nrv*256 + nrv;
        for(int i=0; i<vi->format->numPlanes; i++)
        {
            nrvs.push_back(nrv);
        }
    }
    ResampleData data;
    fmtc->resample_data_init(&data);
    data.w  = w;
    data.h  = h;
    data.sx = sx;
    data.sy = sy;
    data.sw = sw;
    data.sh = sh;
    data.kernel = kernel.c_str();
    data.taps   = taps;
    data.a1     = a1;
    data.a2     = a2;
    data.invks  = invks;
    data.invkstaps = invkstaps;
    data.css = css.c_str();
    for(auto plane: planes)
        data.planes.push_back(plane);
    data.center  = center;
    data.cplace  = cplace.c_str();
    data.cplaces = cplaces.c_str();
    data.cplaced = cplaced.c_str();
    data.interlaced  = interlaced;
    data.interlacedd = interlacedd;
    data.tff  = tff;
    data.tffd = tffd;
    data.flt  = flt;

    VSNodeRef* main = fmtc->resample(src, &data);
    VSNodeRef* last, *out;
    if(nrf)
    {
        data.kernel = "gauss";
        data.a1.push_back(100);

        VSNodeRef* nrng = fmtc->resample(src, &data);

        // To do: use a simple frame blending instead of Merge
        std::vector<int> modes{1};
        last = rgvs->repair(main, nrng, modes);
        if(nrb)
        {
            std::vector<float> colors = nrvs;
            VSNodeRef* nrm = std->blankClip(main, colors);
            last = std->maskedMerge(main, last, nrm);
            vsapi->freeNode(nrm);
        }
        vsapi->freeNode(main);
        vsapi->freeNode(nrng);

    }
    else
    {
        last = main;
    }

    const VSVideoInfo *lastVi = vsapi->getVideoInfo(last);
    if(lastVi->format->bitsPerSample == bits && fulls == NONE && fulld == NONE && dmode == NONE && ampo == NONE && ampn == NONE && dyn == NONE && staticnoise == NONE && patsize == NONE)
    {
        return last;
    }
    else
    {
        std::vector<int64_t> planes2;
        if(planes.empty())
        {
            for(int i=0; i<lastVi->format->numPlanes; i++)
            {
                planes2.push_back(i);
            }
        }
        else
        {
            int len = planes.size();
            while(len < lastVi->format->numPlanes)
            {
                planes.push_back(planes[len - 1]);
            }
            for(int i=0; i<lastVi->format->numPlanes; i++)
            {
                if(planes[i] != 1)
                    planes2.push_back(i);
            }
        }
        BitdepthData bitData;
        fmtc->bitdepth_data_init(&bitData);
        bitData.bits = bits;
        bitData.planes = planes2;
        bitData.fulls = fulls;
        bitData.fulld = fulld;
        bitData.dmode = dmode;
        bitData.ampo = ampo;
        bitData.ampn = ampn;
        bitData.dyn = dyn;
        bitData.staticnoise = staticnoise;
        bitData.patsize = patsize;
        out = fmtc->bitdepth(last, &bitData);
        vsapi->freeNode(last);
        return out;
    }
}

VSNodeRef* MiniQTGMC::DitherLumaRebuild(VSNodeRef* src, double s0, double c, bool chroma)
{
    const VSVideoInfo *vi = vsapi->getVideoInfo(src);
    int shift = vi->format->bitsPerSample - 8;

    bool isGray = (vi->format->colorFamily == cmGray);//false

    double k = (s0 - 1) * c;
    std::string t = str_format("x %d - %d / 0 max 1 min", 16<<shift, 219<<shift);
    double c1 = 1 + c;
    double c2 = c1 * c;
    std::string e = str_format("%f %f %f ", k, c1, c2) + t + str_format(" %f + / - * ", c) + t + str_format(" 1 %f - * + %d *", k, 256 << shift);

    std::vector<VSNodeRef*> exprNodes{src};
    std::vector<const std::string> exprs{e};
    if(!isGray)
    {
        int neutral = 128 << shift;
        std::string str = chroma?str_format("x %d - 128 * 112 / %d +", neutral, neutral):"";
        exprs.push_back(str);
    }
    return std->expr(exprNodes, exprs);
}

VSNodeRef* MiniQTGMC::setTextOut(VSNodeRef* src, std::string str)
{
    return text->Text(src, str.c_str());
}

VSNodeRef* MiniQTGMC::process(bool showSettings)
{
    VSNodeRef* out;
    std::vector<int> planes;
    VSNodeRef *edi = nnedi3->nnedi3(input, /*field=*/3, planes, /*dh=*/false, /*nsize=*/mArgs.NNSize, 
                                    /*nns=*/mArgs.NNeurons, /*qual=*/1, /*etype*/0, /*pscrn*/0);
    VSNodeRef* bobbed = Bob(input, 0, 0.5, true);
    std::vector<int> CMplanes{0, 1, 2};

    VSNodeRef* ts_bobbed = SCDetect(bobbed, 28/255);
    std::vector<double> ts1_weights(3, 1.0);
    std::vector<double> ts2_weights(5, 1.0);
    VSNodeRef* ts1 = misc->averageFrames(ts_bobbed, ts1_weights, true, CMplanes);
    VSNodeRef* ts2 = misc->averageFrames(ts_bobbed, ts2_weights, true, CMplanes);

    std::vector<float> tmpWeights{0.357, 0.0};
    std::vector<float> tmpWeights2{0.125, 0.0};
    VSNodeRef* binomial0 = std->merge(std->merge(ts1, ts2, tmpWeights), bobbed, tmpWeights2);

    VSNodeRef* repair0 = QTGMC_KeepOnlyBobShimmerFixes(binomial0, bobbed, mArgs.Rep0, true);

    std::vector<float> matrix{1, 2, 1, 2, 4, 2, 1, 2, 1};
    VSNodeRef* spatialBlur = Resize(std->convolution(repair0, matrix, CMplanes), w, h, 
                                   /*sx*/std::vector<double>(), /*sy*/std::vector<double>(), 
                                   /*sw*/std::vector<double>{w + epsilon}, /*sh*/std::vector<double>{h + epsilon}, 
                                   /*kernel*/"gauss", /*a1*/std::vector<double>{2}, std::vector<double>()/*a2*/, /*dmode=*/1);

    std::vector<float> tmpWeights3{0.1, 0.0};
    spatialBlur = std->merge(spatialBlur, repair0, tmpWeights3);
    int i, j;
    i = scale(3, peak);
    std::string expr1 = str_format("x %d + y < x %d + x %d - y > x %d - y ? ?", i, i, i, i);
    std::vector<VSNodeRef*> exprNodes1{repair0, bobbed};
    std::vector<const std::string> exprs1{expr1, ""};
    VSNodeRef* tweaked = std->expr(exprNodes1, exprs1);

    i = scale(7, peak);
    j = scale(2, peak);
    std::string expr2 = str_format("x %d + y < x %d + x %d - y > x %d - x 51 * y 49 * + 100 / ? ?", i, j, i, j);
    std::vector<VSNodeRef*> exprNodes2{spatialBlur, tweaked};
    std::vector<const std::string> exprs2{expr2, ""};
    VSNodeRef* srchClip = std->expr(exprNodes2, exprs2);

    VSNodeRef* tmpNode = DitherLumaRebuild(srchClip, 1);
    VSNodeRef* srchSuper = mv->super(tmpNode, /*pel*/2, /*sharp*/2,/*hpad*/hpad, /*vpad*/vpad, /*chroma*/true);
    analyseData ad = 
    {
        .blksize     = mArgs.BlockSize,
        .overlap     = mArgs.Overlap,
        .search      = mArgs.Search,
        .searchparam = mArgs.SearchParam,
        .pelsearch   = mArgs.PelSearch,
        .truemotion  = false,
        .lambda      = 400,
        .lsad        = 400,
        .pnew        = 25,
        .plevel      = 0,
        .global      = true,
        .dct         = 0,
        .chroma      = true,
        //.isb,
        //.delta,
    };
    ad.isb   = true;
    ad.delta = 1;
    VSNodeRef* bVec1 = mv->analyse(srchSuper, ad);
    ad.isb   = false;
    ad.delta = 1;
    VSNodeRef* fVec1 = mv->analyse(srchSuper, ad);
    ad.isb   = true;
    ad.delta = 2;
    VSNodeRef* bVec2 = mv->analyse(srchSuper, ad);
    ad.isb   = false;
    ad.delta = 2;
    VSNodeRef* fVec2 = mv->analyse(srchSuper, ad);

    VSNodeRef* ediSuper = mv->super(edi, /*pel=*/2, /*sharp=*/2, /*hpad=*/hpad, /*vpad=*/vpad, /*chroma*/true, /*levels=*/1);
    VSNodeRef* degrain1 = mv->degrain1(edi, ediSuper, bVec1, fVec1, /*thsad=*/640, /*thscd1=*/180, /*thscd2=*/98); // 0.00  0.33  0.33  0.33  0.00
    //VSNodeRef* degrain2 = mv->degrain1(edi, ediSuper, bVec2, fVec2, /*thsad=*/640, /*thscd1=*/180, /*thscd2=*/98); //0.33  0.00  0.33  0.00  0.33
    if(showSettings)
    {
        std::string str = str_format(
                  "TR0=%d | TR1=%d | TR2=%d | Rep0=%d | Rep1=%d | Rep2=%d | RepChroma=%d | EdiMode=%s | NNSize=%d | NNeurons=%d | EdiQual=%d | EdiMaxD=%d | " \
                   "ChromaEdi=%s | Sharpness=%f | SMode=%d | SLMode=%d | SLRad=%d | SOvs=%d | SVThin=%f | Sbb=%d | SrchClipPP=%d | SubPel=%d | " \
                   "SubPelInterp=%d | BlockSize=%d | Overlap=%d | Search=%d | SearchParam=%d | PelSearch=%d | ChromaMotion=%d | TrueMotion=%d | Lambda=%d | " \
                   "LSAD=%d | PNew=%d | PLevel=%d | GlobalMotion=%d | DCT=%d | ThSAD1=%d | ThSAD2=%d | ThSCD1=%d | ThSCD2=%d | SourceMatch=%d | " \
                   "MatchPreset=%s | MatchEdi=%s | MatchPreset2=%s | MatchEdi2=%s | MatchTR2=%d | MatchEnhance=%f | Lossless=%d | NoiseProcess=%d | " \
                   "Denoiser=%s | FftThreads=%d | DenoiseMC=%d | NoiseTR=%d | Sigma=%f | ChromaNoise=%d | ShowNoise=%d | GrainRestore=%f | NoiseRestore=%f | " \
                   "NoiseDeint=%s | StabilizeNoise=%d | InputType=%d | ProgSADMask=%f | FPSDivisor=%d | ShutterBlur=%d | ShutterAngleSrc=%f | " \
                   "ShutterAngleOut=%f | SBlurLimit=%d | Border=%d | Precise=%d | Preset=%s | Tuning=%s",
                    mArgs.TR0, mArgs.TR1, mArgs.TR2, mArgs.Rep0, mArgs.Rep1, mArgs.Rep2, mArgs.RepChroma, mArgs.EdiMode.c_str(), mArgs.NNSize, mArgs.NNeurons,
                    mArgs.EdiQual, mArgs.EdiMaxD, mArgs.ChromaEdi.c_str(), mArgs.Sharpness, mArgs.SMode,
                    mArgs.SLMode, mArgs.SLRad, mArgs.SOvs, mArgs.SVThin, mArgs.Sbb, mArgs.SrchClipPP, mArgs.SubPel, mArgs.SubPelInterp,
                    mArgs.BlockSize, mArgs.Overlap, mArgs.Search, mArgs.SearchParam, mArgs.PelSearch, mArgs.ChromaMotion,
                    mArgs.TrueMotion, mArgs.Lambda, mArgs.LSAD, mArgs.PNew, mArgs.PLevel, mArgs.GlobalMotion, mArgs.DCT, mArgs.ThSAD1,
                    mArgs.ThSAD2, mArgs.ThSCD1, mArgs.ThSCD2, mArgs.SourceMatch, mArgs.MatchPreset.c_str(), mArgs.MatchEdi.c_str(),
                    mArgs.MatchPreset2.c_str(), mArgs.MatchEdi2.c_str(), mArgs.MatchTR2, mArgs.MatchEnhance, mArgs.Lossless, mArgs.NoiseProcess, mArgs.Denoiser.c_str(),
                    mArgs.FftThreads, mArgs.DenoiseMC, mArgs.NoiseTR, mArgs.Sigma,
                    mArgs.ChromaNoise, mArgs.ShowNoise, mArgs.GrainRestore, mArgs.NoiseRestore, mArgs.NoiseDeint.c_str(), mArgs.StabilizeNoise, 
                    mArgs.InputType, mArgs.ProgSADMask, mArgs.FPSDivisor, mArgs.ShutterBlur,
                    mArgs.ShutterAngleSrc, mArgs.ShutterAngleOut, mArgs.SBlurLimit, mArgs.Border, mArgs.Precise, mArgs.Preset.c_str(), mArgs.Tuning.c_str());

        out = setTextOut(degrain1, str);
        vsapi->freeNode(degrain1);
    }
    else
    {
        out = degrain1;
    }

    vsapi->freeNode(edi);
    vsapi->freeNode(bobbed);
    vsapi->freeNode(ts_bobbed);
    vsapi->freeNode(ts1);
    vsapi->freeNode(ts2);
    vsapi->freeNode(binomial0);
    vsapi->freeNode(repair0);
    vsapi->freeNode(spatialBlur);
    vsapi->freeNode(tweaked);
    vsapi->freeNode(srchClip);
    vsapi->freeNode(tmpNode);
    vsapi->freeNode(srchSuper);
    vsapi->freeNode(bVec1);
    vsapi->freeNode(fVec1);
    vsapi->freeNode(bVec2);
    vsapi->freeNode(fVec2);
    vsapi->freeNode(ediSuper);
    //vsapi->freeNode(degrain2);

    return out;
}