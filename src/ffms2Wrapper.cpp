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
   File:		ffms2Wrapper.cpp
   Author:		Gan Qiuye(ganqiuye@163.com)
   Date:		2021-05-10  14:40
   Version:		1.0
   Description:
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
