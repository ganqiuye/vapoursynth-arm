/*================================================================
*
*   File:		fmtcWrapper.h
*   Author:		Ding Binyuang(dbinyuan@163.com)
*   Date:		2021-05-12  16:28
*   Version:		1.0
*   Description:
*
================================================================*/
#ifndef __FMTC_WRAPPER_H__
#define __FMTC_WRAPPER_H__

#include <vector>
#include <string>
#include "VapourSynth.h"
#include "basePluginWrapper.h"

typedef struct
{
    int64_t w;
    int64_t h;
    std::vector<double> sx;
    std::vector<double> sy;
    std::vector<double> sw;
    std::vector<double> sh;
    double scale;
    double scaleh;
    double scalev;
    std::string kernel;
    std::string kernelh;
    std::string kernelv;
    std::vector<double> impulse;
    std::vector<double> impulseh;
    std::vector<double> impulsev;
    std::vector<int64_t> taps;
    std::vector<int64_t> tapsh;
    std::vector<int64_t> tapsv;
    std::vector<double> a1;
    std::vector<double> a2;
    std::vector<double> a3;
    std::vector<int64_t> kovrspl;
    std::vector<double> fh;
    std::vector<double> fv;
    std::vector<int64_t> cnorm;
    std::vector<double> totalh;
    std::vector<double> totalv;
    std::vector<int64_t> invks;
    std::vector<int64_t> invksh;
    std::vector<int64_t> invksv;
    std::vector<int64_t> invkstaps;
    std::vector<int64_t> invkstapsh;
    std::vector<int64_t> invkstapsv;
    int64_t csp;
    std::string css;
    std::vector<double> planes;
    int64_t fulls;
    int64_t fulld;
    std::vector<int64_t> center;
    std::string cplace;
    std::string cplaces;
    std::string cplaced;
    int64_t interlaced;
    int64_t interlacedd;
    int64_t tff;
    int64_t tffd;
    int64_t flt;
    int64_t cpuopt;
} ResampleData;

typedef struct
{
    int64_t csp;
    int64_t bits;
    int64_t flt;
    std::vector<int64_t> planes;
    int64_t fulls;
    int64_t fulld;
    int64_t dmode;
    double ampo;
    double ampn;
    int64_t dyn;
    int64_t staticnoise;
    int64_t cpuopt;
    int64_t patsize;
} BitdepthData;

struct FmtcWrapper : public BasePluginWrapper
{
    FmtcWrapper(const VSAPI* vsapi, VSCore *core):BasePluginWrapper(vsapi, core, "fmtconv"){fmtcPlugin = getPlugin();};
    void resample_data_init(ResampleData* data);
    VSNodeRef* resample(VSNodeRef* node, ResampleData* data);

    void bitdepth_data_init(BitdepthData* data);
    VSNodeRef* bitdepth(VSNodeRef* node, BitdepthData* data);

private:
    VSPlugin* fmtcPlugin;
    DISALLOW_COPY_AND_ASSIGN(FmtcWrapper);
};

#endif //__FMTC_WRAPPER_H__
