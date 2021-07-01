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
   File:		nnedi3Wrapper.h
   Author:		Gan Qiuye(ganqiuye@163.com)
   Date:		2021-05-10  16:28
   Version:		1.0
   Description:
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