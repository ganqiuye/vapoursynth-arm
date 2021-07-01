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
   File:		ffms2Wrapper.h
   Author:		Gan Qiuye(ganqiuye@163.com)
   Date:		2021-05-10  14:41
   Version:		1.0
   Description:
================================================================*/
#ifndef __FFMS2_WRAPPER_H__
#define __FFMS2_WRAPPER_H__

#include "VapourSynth.h"
#include "basePluginWrapper.h"

struct FFms2Wrapper : public BasePluginWrapper
{
    FFms2Wrapper(const VSAPI* vsapi, VSCore *core):BasePluginWrapper(vsapi, core, "com.vapoursynth.ffms2"){ffm2Plugin = getPlugin();}
    virtual ~FFms2Wrapper() {}

    VSNodeRef* setSource(const char* source, int track = 0, int cache = 0, const char* cachefile = nullptr, int fpsnum = 0, int fpsden = 0, int threads = 0, const char* timecodes = nullptr, int seekmode = 0, int width = 0, int height = 0, const char* resizer = nullptr, int format = 0, int alpha = 0);
    //void createIndex(const char* source, const char* cachefile = nullptr, int* indextracks = nullptr, int errorhandling = 0, int overwrite = 0);
    const char* getVersion();
    //GetLogLevel()
    //SetLogLevel()

private:
    VSPlugin* ffm2Plugin;
    DISALLOW_COPY_AND_ASSIGN(FFms2Wrapper);
};

#endif //__FFMS2_WRAPPER_H__
