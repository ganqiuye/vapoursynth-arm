/*================================================================
*
*   File:		textWrapper.h
*   Author:		Ding Binyuang(dbinyuan@163.com)
*   Date:		2021-05-14  13:40
*   Version:		1.0
*   Description:
*
================================================================*/
#ifndef __TEXT_WRAPPER_H__
#define __TEXT_WRAPPER_H__

#include <vector>
#include "VapourSynth.h"
#include "basePluginWrapper.h"

struct TextWrapper : public BasePluginWrapper
{
    TextWrapper(const VSAPI* vsapi, VSCore *core):BasePluginWrapper(vsapi, core, "com.vapoursynth.text"){textPlugin = getPlugin();}
    VSNodeRef* Text(VSNodeRef* node, const char *text, int64_t alignment = 7, int64_t scale = 1);

private:
    VSPlugin* textPlugin;
    DISALLOW_COPY_AND_ASSIGN(TextWrapper);
};

#endif //__TEXT_WRAPPER_H__
