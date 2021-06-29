/*================================================================
*
*   File:		nnedi3Wrapper.h
*   Author:		Gan Qiuye(ganqiuye@163.com)
*   Date:		2021-05-10  16:28
*   Version:		1.0
*   Description:
*
================================================================*/
#ifndef __NNEDI3_WRAPPER_H__
#define __NNEDI3_WRAPPER_H__
#include <vector>
#include "VapourSynth.h"
#include "basePluginWrapper.h"

struct Nnedi3Wrapper : public BasePluginWrapper
{
    Nnedi3Wrapper(const VSAPI* vsapi, VSCore *core):BasePluginWrapper(vsapi, core, "com.deinterlace.nnedi3"){nnedi3Plugin = getPlugin();}
    virtual ~Nnedi3Wrapper() {}
    VSNodeRef* nnedi3(VSNodeRef* node, int field, std::vector<int> planes = {}, bool dh = false, int nsize = 6, 
                      int nns = 1, int qual = 1, int etype = 0, int pscrn = 2, bool opt = true, 
                      bool int16_prescreener = true, bool int16_predictor = true, int exp = 0, 
                      bool show_mask = false, bool combed_only = false);

private:
    VSPlugin* nnedi3Plugin;
    DISALLOW_COPY_AND_ASSIGN(Nnedi3Wrapper);
};

#endif //__NNEDI3_WRAPPER_H__