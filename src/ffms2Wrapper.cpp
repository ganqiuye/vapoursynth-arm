/*================================================================
*
*   File:		ffms2Wrapper.cpp
*   Author:		Gan Qiuye(ganqiuye@163.com)
*   Date:		2021-05-10  14:40
*   Version:		1.0
*   Description:
*
================================================================*/

#include "ffms2Wrapper.h"

VSNodeRef* FFms2Wrapper::setSource(const char* source, int track, int cache, const char* cachefile, int fpsnum, int fpsden, int threads, \
           const char* timecodes, int seekmode, int width, int height, const char* resizer, int format, int alpha)
{
    VSMap *args, *ret;
    args = vsapi->createMap();
    vsapi->propSetData(args, "source", source, -1, paReplace);
    vsapi->propSetInt(args, "track", track, paReplace);
    vsapi->propSetInt(args, "cache", cache, paReplace);
    //....
    ret = vsapi->invoke(ffm2Plugin, "Source", args);
    VSNodeRef *node = vsapi->propGetNode(ret, "clip", 0, nullptr);
    vsapi->freeMap(args);
    vsapi->freeMap(ret);

    return node;
}

const char* FFms2Wrapper::getVersion()
{
    VSMap *args, *ret;
    int err;
    args = vsapi->createMap();
    ret  = vsapi->invoke(ffm2Plugin, "Version", args);
    const char *version = vsapi->propGetData(ret, "version", 0, &err);
    vsapi->freeMap(args);
    vsapi->freeMap(ret);
    return version;
}
