/*================================================================
*
*   File:		mvtoolsWrapper.cpp
*   Author:		Bill Cong(cjcbill@gmail.com)
*   Date:		2021-05-10  16:28
*   Version:		1.0
*   Description:
*
================================================================*/

#include "mvtoolsWrapper.h"
#include "common.h"

VSNodeRef* MvtoolsWrapper::analyse(VSNodeRef* node, analyseData ad)
{
    VSMap *args, *ret;
    args = vsapi->createMap();
    vsapi->propSetNode(args, "super", node, paReplace);
    vsapi->propSetInt(args, "blksize", ad.blksize, paReplace);
    vsapi->propSetInt(args, "overlap", ad.overlap, paReplace);
    vsapi->propSetInt(args, "search", ad.search, paReplace);
    vsapi->propSetInt(args, "searchparam", ad.searchparam, paReplace);
    vsapi->propSetInt(args, "pelsearch", ad.pelsearch, paReplace);
    vsapi->propSetInt(args, "truemotion", ad.truemotion, paReplace);
    vsapi->propSetInt(args, "lambda", ad.lambda, paReplace);
    vsapi->propSetInt(args, "lsad", ad.lsad, paReplace);
    vsapi->propSetInt(args, "pnew", ad.pnew, paReplace);
    vsapi->propSetInt(args, "plevel", ad.plevel, paReplace);
    vsapi->propSetInt(args, "global", ad.global, paReplace);
    vsapi->propSetInt(args, "dct", ad.dct, paReplace);
    vsapi->propSetInt(args, "chroma", ad.chroma, paReplace);
    vsapi->propSetInt(args, "isb", ad.isb, paReplace);
    vsapi->propSetInt(args, "delta", ad.delta, paReplace);

    ret = vsapi->invoke(mvtoolsPlugin, "Analyse", args);

    DEBUG_SHOW(0);

    VSNodeRef *retnode = vsapi->propGetNode(ret, "clip", 0, nullptr);
    vsapi->freeMap(args);
    vsapi->freeMap(ret);
    return retnode;
}


VSNodeRef* MvtoolsWrapper::super(VSNodeRef* node,
                                 int pel,
                                 int sharp,
                                 int hpad,
                                 int vpad,
                                 int chroma,
                                 int levels)
{
    VSMap *args, *ret;
    args = vsapi->createMap();
    vsapi->propSetNode(args, "clip", node, paReplace);
    vsapi->propSetInt(args, "pel", pel, paReplace);
    vsapi->propSetInt(args, "sharp", sharp, paReplace);
    vsapi->propSetInt(args, "hpad", hpad, paReplace);
    vsapi->propSetInt(args, "vpad", vpad, paReplace);
    vsapi->propSetInt(args, "chroma", chroma, paReplace);
    vsapi->propSetInt(args, "levels", levels, paReplace);
    ret = vsapi->invoke(mvtoolsPlugin, "Super", args);

    DEBUG_SHOW(0);

    VSNodeRef *retnode = vsapi->propGetNode(ret, "clip", 0, nullptr);
    vsapi->freeMap(args);
    vsapi->freeMap(ret);
    return retnode;
}

VSNodeRef* MvtoolsWrapper::degrain1(VSNodeRef* node,
                                    VSNodeRef* super,
                                    VSNodeRef* mvbw,
                                    VSNodeRef* mvfw,
                                    int thsad,
                                    int thscd1,
                                    int thscd2)
{
    VSMap *args, *ret;
    args = vsapi->createMap();
    vsapi->propSetNode(args, "clip", node, paReplace);
    vsapi->propSetNode(args, "super", super, paReplace);
    vsapi->propSetNode(args, "mvbw", mvbw, paReplace);
    vsapi->propSetNode(args, "mvfw", mvfw, paReplace);
    vsapi->propSetInt(args, "thsad", thsad, paReplace);
    vsapi->propSetInt(args, "thscd1", thscd1, paReplace);
    vsapi->propSetInt(args, "thscd2", thscd2, paReplace);
    ret = vsapi->invoke(mvtoolsPlugin, "Degrain1", args);
    
    DEBUG_SHOW(0);

    VSNodeRef *retnode = vsapi->propGetNode(ret, "clip", 0, nullptr);
    vsapi->freeMap(args);
    vsapi->freeMap(ret);
    return retnode;
}

