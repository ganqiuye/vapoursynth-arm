/*================================================================
MIT License

Copyright (c) 2021 ganqiuye

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
   File:		QTGMC.h
   Author:		Gan Qiuye(ganqiuye@163.com)
   Date:		2021-05-12  13:37
   Version:		1.0
   Description:
================================================================*/

#include "VapourSynth.h"
#include "nnedi3Wrapper.h"
#include "stdWrapper.h"
#include "miscWrapper.h"
#include "fmtcWrapper.h"
#include "mvtoolsWrapper.h"
#include "removeGrainWrapper.h"
#include "textWrapper.h"
#include <string>

struct QtgmsArgs
{
    enum PresetType
    {
        Placebo,   // 0
        Very_Slow, // 1
        Slower,    // 2
        Slow,      // 3
        Medium,    // 4
        Fast,      // 5
        Faster,    // 6
        very_Fast, // 7
        Super_Fast, // 8
        Ultra_Fast, // 9
        Draft,     // 10
    };

    enum EdiModeType
    {
        None,
        NNEDI2,
        NNEDI3,
        RepYadif,
        Bob
    };

    std::string Preset;
    int TR0;
    int TR1;
    int TR2;
    int Rep0;
    int Rep1;
    int Rep2;
    std::string EdiMode;
    int RepChroma;
    int NNSize;
    int NNeurons;
    int EdiQual;
    int EdiMaxD;
    std::string ChromaEdi;
    int EdiThreads;
    //clip "EdiExt";
    float Sharpness;
    int SMode;
    int SLMode;
    int SLRad;
    int SOvs;
    float SVThin;
    int Sbb;
    int SrchClipPP;
    int SubPel;
    int SubPelInterp;
    int BlockSize;
    int Overlap;
    int Search;
    int SearchParam;
    int PelSearch;
    int ChromaMotion;
    int TrueMotion;
    int Lambda;
    int LSAD;
    int PNew;
    int PLevel;
    int GlobalMotion;
    int DCT;
    int ThSAD1;
    int ThSAD2;
    int ThSCD1;
    int ThSCD2;
    int SourceMatch;
    std::string MatchPreset;
    std::string MatchEdi;
    std::string MatchPreset2;
    std::string MatchEdi2;
    int MatchTR2;
    float MatchEnhance;
    int Lossless;
    int NoiseProcess;
    float EZDenoise;
    float EZKeepGrain;
    std::string NoisePreset;
    std::string Denoiser;
    int FftThreads;
    int DenoiseMC;
    int NoiseTR;
    float Sigma;
    int ChromaNoise;
    int ShowNoise;//?
    float GrainRestore;
    float NoiseRestore;
    std::string NoiseDeint;
    int StabilizeNoise;
    int InputType;
    float ProgSADMask;
    int FPSDivisor;
    int ShutterBlur;
    float ShutterAngleSrc;
    float ShutterAngleOut;
    int SBlurLimit;
    int Border;
    int Precise;
    std::string Tuning;
    int ShowSettings;
    int DetailRestore;//?
    int MotionBlur;//?
    int MBlurLimit;//?
    int NoiseBypass;//?
    float Str;
    float Amp;
    int TV_range;
    int useEdiExt;
    int DftDither;
    int Lsbd;
    int Lsb;
};

class QTGMC
{
public:

    QTGMC(){}
    QTGMC(const VSAPI* vsapi, VSCore *core, VSNodeRef* node, QtgmsArgs* args);
    ~QTGMC(){}

private:
    const VSAPI* vsapi;
    VSCore *core;
    VSNodeRef* input;
    QtgmsArgs mArgs;
    DISALLOW_COPY_AND_ASSIGN(QTGMC);
};


class MiniQTGMC
{
public:
    MiniQTGMC(const VSAPI* vsapi, VSCore *core, VSNodeRef* inputNode);
    ~MiniQTGMC();

    VSNodeRef* process(bool showSettings = false);


private:
    const VSAPI* vsapi;
    VSCore *core;
    VSNodeRef* input;
    QtgmsArgs mArgs;
    int neutral;
    int peak;
    bool isGray;
    float epsilon;
    int hpad;
    int vpad;
    int bitsPerSample;
    int w;
    int h;
    Nnedi3Wrapper *nnedi3;
    StdWrapper *std;
    FmtcWrapper* fmtc;
    MiscWrapper* misc;
    MvtoolsWrapper* mv;
    RemoveGrainWrapper *rgvs;
    TextWrapper* text;

    VSNodeRef* Bob(VSNodeRef* clip, double b = 1/3, double c = 1/3, bool tff = true);
    VSNodeRef* SCDetect(VSNodeRef* clip, double threshold = -1);
    VSNodeRef* QTGMC_KeepOnlyBobShimmerFixes(VSNodeRef* input, VSNodeRef* Ref, int Rep = 1, bool Chroma = true);
    VSNodeRef* Resize(VSNodeRef* src, int w, int h, std::vector<double> sx = std::vector<double>(), std::vector<double> sy = std::vector<double>(),
                      std::vector<double> sw = std::vector<double>(), std::vector<double> sh = std::vector<double>(),
                      std::string kernel="", std::vector<double> a1= std::vector<double>(), std::vector<double> a2= std::vector<double>(),
                      int dmode = NONE, std::vector<int64_t> invks = std::vector<int64_t>(), std::vector<int64_t> invkstaps = std::vector<int64_t>(),
                      std::vector<int> planes = std::vector<int>(), std::vector<int64_t> taps = std::vector<int64_t>(),
                      bool noring = false, std::string css ="", std::vector<int64_t> center = std::vector<int64_t>(),
                      std::string cplace = "", std::string cplaces = "", std::string cplaced = "", int interlaced = NONE,
                      int interlacedd = NONE, int tff = NONE, int tffd = NONE, int flt = NONE,
                      int bits = NONE, int fulls = NONE, int fulld = NONE, int ampo = NONE, int ampn = NONE,
                      int dyn = NONE, int staticnoise = NONE, int patsize = NONE);
    VSNodeRef* DitherLumaRebuild(VSNodeRef *src, double s0 = 2.0, double c = 0.0625, bool chroma = true);

    VSNodeRef* setTextOut(VSNodeRef* src, std::string text);
    DISALLOW_COPY_AND_ASSIGN(MiniQTGMC);
};

