/*================================================================
*
*   File:		basePluginWrapper.cpp
*   Author:		Gan Qiuye(ganqiuye@163.com)
*   Date:		2021-05-11  10:27
*   Version:		1.0
*   Description:
*
================================================================*/

#include "basePluginWrapper.h"

BasePluginWrapper::BasePluginWrapper(const VSAPI* vsapi, VSCore *core, const char* pluginStr)
{
    this->vsapi = vsapi;
    this->core  = core;
    basePlugin = vsapi->getPluginById(pluginStr, core);
}