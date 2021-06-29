/*================================================================
*
*   File:		ffms2Wrapper.h
*   Author:		Gan Qiuye(ganqiuye@163.com)
*   Date:		2021-05-10  14:41
*   Version:		1.0
*   Description:
*
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
