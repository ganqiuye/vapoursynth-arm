/*================================================================
*
*   File:		mvtoolsWrapper.h
*   Author:		Bill Cong(cjcbill@gmail.com)
*   Date:		2021-05-12  16:28
*   Version:		1.0
*   Description:
*
================================================================*/
#ifndef __MVTOOLS_WRAPPER_H__
#define __MVTOOLS_WRAPPER_H__

#include "VapourSynth.h"
#include "basePluginWrapper.h"

typedef struct AnalyseData
{
    int blksize;
    int overlap;
    int search;
    int searchparam;
    int pelsearch;
    int truemotion;
    int lambda;
    int lsad;
    int pnew;
    int plevel;
    bool global;
    int dct;
    bool chroma;
    bool isb;
    int delta;
} analyseData;

struct MvtoolsWrapper : public BasePluginWrapper
{
    MvtoolsWrapper(const VSAPI* vsapi, VSCore *core):
        BasePluginWrapper(vsapi, core, "com.nodame.mvtools")
    {
        mvtoolsPlugin = getPlugin();
    }
    virtual ~MvtoolsWrapper() {}
    VSNodeRef* analyse(VSNodeRef* node, analyseData ad);
    VSNodeRef* super(VSNodeRef* node,
                     int pel = 2,
                     int sharp = 2,
                     int hpad = 16,
                     int vpad = 16,
                     int chroma = true,
                     int levels = 0);
    VSNodeRef* degrain1(VSNodeRef* node,
                        VSNodeRef* super,
                        VSNodeRef* mvbw,
                        VSNodeRef* mvfw,
                        int thsad = 400,
                        int thscd1 = 400,
                        int thscd2 = 130);

private:
    VSPlugin* mvtoolsPlugin;
    DISALLOW_COPY_AND_ASSIGN(MvtoolsWrapper);
};

#endif //__MVTOOLS_WRAPPER_H__
