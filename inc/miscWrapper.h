/*================================================================
*
*   File:		miscWrapper.h
*   Author:		Ding Binyuang(dbinyuan@163.com)
*   Date:		2021-05-11  14:38
*   Version:		1.0
*   Description:
*
================================================================*/
#ifndef __MISC_WRAPPER_H__
#define __MISC_WRAPPER_H__

#include <vector>
#include "VapourSynth.h"
#include "basePluginWrapper.h"

struct MiscWrapper : public BasePluginWrapper
{
    MiscWrapper(const VSAPI* vsapi, VSCore *core):BasePluginWrapper(vsapi, core, "com.vapoursynth.misc"){miscPlugin = getPlugin();}
    VSNodeRef* SCDetect(VSNodeRef* node, double threshold = 0.1);
    VSNodeRef* averageFrames(VSNodeRef* node, std::vector<double> weights, int scenechange, std::vector<int> planes = {});

private:
    VSPlugin* miscPlugin;
    DISALLOW_COPY_AND_ASSIGN(MiscWrapper);
};

#endif //__MISC_WRAPPER_H__
