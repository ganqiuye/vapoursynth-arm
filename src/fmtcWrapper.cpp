/*================================================================
*
*   File:		fmtcWrapper.cpp
*   Author:		Ding Binyuang(dbinyuan@163.com)
*   Date:		2021-05-12  16:28
*   Version:		1.0
*   Description:
*
================================================================*/

#include <string.h>
#include "common.h"
#include "fmtcWrapper.h"

void FmtcWrapper::resample_data_init(ResampleData* data)
{
    data->w           = NONE;
    data->h           = NONE;
    data->scale       = NONE;
    data->scaleh      = NONE;
    data->scalev      = NONE;
    data->csp         = NONE;
    data->fulls       = NONE;
    data->fulld       = NONE;
    data->interlaced  = NONE;
    data->interlacedd = NONE;
    data->tff         = NONE;
    data->tffd        = NONE;
    data->flt         = NONE;
    data->cpuopt      = NONE;
}

VSNodeRef* FmtcWrapper::resample(VSNodeRef* node, ResampleData* data)
{
    VSMap *args, *ret;
    args = vsapi->createMap();

    vsapi->propSetNode(args, "clip", node, paReplace);
    if (data->w != NONE)
        vsapi->propSetInt(args, "w", data->w, paReplace);
    if (data->h != NONE)
        vsapi->propSetInt(args, "h", data->h, paReplace);

    if (!data->sx.empty())
        for (auto sx: data->sx)
            vsapi->propSetFloat(args, "sx", sx, paAppend);
    if (!data->sy.empty())
        for (auto sy: data->sy)
            vsapi->propSetFloat(args, "sy", sy, paAppend);
    if (!data->sw.empty())
        for (auto sw: data->sw)
            vsapi->propSetFloat(args, "sw", sw, paAppend);
    if (!data->sh.empty())
        for (auto sh: data->sh)
            vsapi->propSetFloat(args, "sh", sh, paAppend);

    if (data->scale != NONE)
        vsapi->propSetFloat(args, "scale", data->scale, paReplace);
    if (data->scaleh != NONE)
        vsapi->propSetFloat(args, "scaleh", data->scaleh, paReplace);
    if (data->scalev != NONE)
        vsapi->propSetFloat(args, "scalev", data->scalev, paReplace);

    if (!data->kernel.empty())
        vsapi->propSetData(args, "kernel", data->kernel.c_str(), -1, paReplace);
    if (!data->kernelh.empty())
        vsapi->propSetData(args, "kernelh", data->kernelh.c_str(), -1, paReplace);
    if (!data->kernelv.empty())
        vsapi->propSetData(args, "kernelv", data->kernelv.c_str(), -1, paReplace);

    if (!data->impulse.empty())
        for (auto impulse: data->impulse)
            vsapi->propSetFloat(args, "impulse", impulse, paAppend);
    if (!data->impulseh.empty())
        for (auto impulseh: data->impulseh)
            vsapi->propSetFloat(args, "impulseh", impulseh, paAppend);
    if (!data->impulsev.empty())
        for (auto impulsev: data->impulsev)
            vsapi->propSetFloat(args, "impulsev", impulsev, paAppend);

    if (!data->taps.empty())
        for (auto taps: data->taps)
            vsapi->propSetInt(args, "taps", taps, paAppend);
    if (!data->tapsh.empty())
        for (auto tapsh: data->tapsh)
            vsapi->propSetInt(args, "tapsh", tapsh, paAppend);
    if (!data->tapsv.empty())
        for (auto tapsv: data->tapsv)
            vsapi->propSetInt(args, "tapsv", tapsv, paAppend);

    if (!data->a1.empty())
        for (auto a1: data->a1)
            vsapi->propSetFloat(args, "a1", a1, paAppend);
    if (!data->a2.empty())
        for (auto a2: data->a2)
            vsapi->propSetFloat(args, "a2", a2, paAppend);
    if (!data->a3.empty())
        for (auto a3: data->a3)
            vsapi->propSetFloat(args, "a3", a3, paAppend);

    if (!data->kovrspl.empty())
        for (auto kovrspl: data->kovrspl)
            vsapi->propSetInt(args, "kovrspl", kovrspl, paAppend);

    if (!data->fh.empty())
        for (auto fh: data->fh)
            vsapi->propSetFloat(args, "fh", fh, paAppend);
    if (!data->fv.empty())
        for (auto fv: data->fv)
            vsapi->propSetFloat(args, "fv", fv, paAppend);

    if (!data->cnorm.empty())
        for (auto cnorm: data->cnorm)
            vsapi->propSetInt(args, "cnorm", cnorm, paAppend);

    if (!data->totalh.empty())
        for (auto totalh: data->totalh)
            vsapi->propSetFloat(args, "totalh", totalh, paAppend);
    if (!data->totalv.empty())
        for (auto totalv: data->totalv)
            vsapi->propSetFloat(args, "totalv", totalv, paAppend);

    if (!data->invks.empty())
        for (auto invks: data->invks)
            vsapi->propSetInt(args, "invks", invks, paAppend);
    if (!data->invksh.empty())
        for (auto invksh: data->invksh)
            vsapi->propSetInt(args, "invksh", invksh, paAppend);
    if (!data->invksv.empty())
        for (auto invksv: data->invksv)
            vsapi->propSetInt(args, "invksv", invksv, paAppend);

    if (!data->invkstaps.empty())
        for (auto invkstaps: data->invkstaps)
            vsapi->propSetInt(args, "invkstaps", invkstaps, paAppend);
    if (!data->invkstapsh.empty())
        for (auto invkstapsh: data->invkstapsh)
            vsapi->propSetInt(args, "invkstapsh", invkstapsh, paAppend);
    if (!data->invkstapsv.empty())
        for (auto invkstapsv: data->invkstapsv)
            vsapi->propSetInt(args, "invkstapsv", invkstapsv, paAppend);

    if (data->csp != NONE)
        vsapi->propSetInt(args, "csp", data->csp, paReplace);
    if (!data->css.empty())
        vsapi->propSetData(args, "css", data->css.c_str(), -1, paReplace);

    if (!data->planes.empty())
        for (auto planes: data->planes)
            vsapi->propSetFloat(args, "planes", planes, paAppend);

    if (data->fulls != NONE)
        vsapi->propSetInt(args, "fulls", data->fulls, paReplace);
    if (data->fulld != NONE)
        vsapi->propSetInt(args, "fulld", data->fulld, paReplace);

    if (!data->center.empty())
        for (auto center: data->center)
            vsapi->propSetInt(args, "center", center, paAppend);

    if (!data->cplace.empty())
        vsapi->propSetData(args, "cplace", data->cplace.c_str(), -1, paReplace);
    if (!data->cplaces.empty())
        vsapi->propSetData(args, "cplaces", data->cplaces.c_str(), -1, paReplace);
    if (!data->cplaced.empty())
        vsapi->propSetData(args, "cplaced", data->cplaced.c_str(), -1, paReplace);

    if (data->interlaced != NONE)
        vsapi->propSetInt(args, "interlaced", data->interlaced, paReplace);
    if (data->interlacedd != NONE)
        vsapi->propSetInt(args, "interlacedd", data->interlacedd, paReplace);
    if (data->tff != NONE)
        vsapi->propSetInt(args, "tff", data->tff, paReplace);
    if (data->tffd != NONE)
        vsapi->propSetInt(args, "tffd", data->tffd, paReplace);
    if (data->flt != NONE)
        vsapi->propSetInt(args, "flt", data->flt, paReplace);
    if (data->cpuopt != NONE)
        vsapi->propSetInt(args, "cpuopt", data->cpuopt, paReplace);

    ret = vsapi->invoke(fmtcPlugin, "resample", args);
    DEBUG_SHOW(0);

    VSNodeRef *retnode = vsapi->propGetNode(ret, "clip", 0, nullptr);

    vsapi->freeMap(args);
    vsapi->freeMap(ret);
    return retnode;
}

void FmtcWrapper::bitdepth_data_init(BitdepthData* data)
{
    data->csp         = NONE;
    data->bits        = NONE;
    data->flt         = NONE;
    data->fulls       = NONE;
    data->fulld       = NONE;
    data->dmode       = NONE;
    data->dyn         = NONE;
    data->staticnoise = NONE;
    data->cpuopt      = NONE;
    data->patsize     = NONE;
    data->ampo        = NONE;
    data->ampn        = NONE;
}

VSNodeRef* FmtcWrapper::bitdepth(VSNodeRef* node, BitdepthData* data)
{
    VSMap *args, *ret;
    args = vsapi->createMap();

    vsapi->propSetNode(args, "clip", node, paReplace);
    if (data->csp != NONE)
        vsapi->propSetInt(args, "csp", data->csp, paReplace);
    if (data->bits != NONE)
        vsapi->propSetInt(args, "bits", data->bits, paReplace);
    if (data->flt != NONE)
        vsapi->propSetInt(args, "flt", data->flt, paReplace);

    if (!data->planes.empty())
        for (auto planes: data->planes)
            vsapi->propSetInt(args, "planes", planes, paAppend);

    if (data->fulls != NONE)
        vsapi->propSetInt(args, "fulls", data->fulls, paReplace);
    if (data->fulld != NONE)
        vsapi->propSetInt(args, "fulld", data->fulld, paReplace);
    if (data->dmode != NONE)
        vsapi->propSetInt(args, "dmode", data->dmode, paReplace);
    if (data->ampo != NONE)
        vsapi->propSetFloat(args, "ampo", data->ampo, paAppend);
    if (data->ampn != NONE)
        vsapi->propSetFloat(args, "ampn", data->ampn, paAppend);

    if (data->dyn != NONE)
        vsapi->propSetInt(args, "dyn", data->dyn, paReplace);
    if (data->staticnoise != NONE)
        vsapi->propSetInt(args, "staticnoise", data->staticnoise, paReplace);
    if (data->cpuopt != NONE)
        vsapi->propSetInt(args, "cpuopt", data->cpuopt, paReplace);
    if (data->patsize != NONE)
        vsapi->propSetInt(args, "patsize", data->patsize, paReplace);

    ret = vsapi->invoke(fmtcPlugin, "bitdepth", args);
    DEBUG_SHOW(0);

    VSNodeRef *retnode = vsapi->propGetNode(ret, "clip", 0, nullptr);

    vsapi->freeMap(args);
    vsapi->freeMap(ret);
    return retnode;
}

