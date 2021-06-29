/*================================================================
*
*   File:		basePluginWrapper.h
*   Author:		Gan Qiuye(ganqiuye@163.com)
*   Date:		2021-05-11  10:27
*   Version:		1.0
*   Description:
*
================================================================*/
#ifndef __BASE_PLUGIN_WRAPPER_H__
#define __BASE_PLUGIN_WRAPPER_H__
#include "VapourSynth.h"
#include <stdio.h>
#include "common.h"

struct BasePluginWrapper
{
    const VSAPI* vsapi;
    VSCore *core;
    BasePluginWrapper(const VSAPI* vsapi, VSCore *core, const char* pluginStr);
    virtual ~BasePluginWrapper() {}
    VSPlugin* getPlugin(){return basePlugin;}

private:
    VSPlugin* basePlugin;
    DISALLOW_COPY_AND_ASSIGN(BasePluginWrapper);
};

#endif //__BASE_PLUGIN_WRAPPER_H__

