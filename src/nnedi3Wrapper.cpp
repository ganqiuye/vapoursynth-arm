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
   File:		nnedi3Wrapper.cpp
   Author:		Gan Qiuye(ganqiuye@163.com)
   Date:		2021-05-10  16:28
   Version:		1.0
   Description:
================================================================*/

#include "nnedi3Wrapper.h"
#include "common.h"

VSNodeRef* Nnedi3Wrapper::nnedi3(VSNodeRef* node, int field, std::vector<int> planes, bool dh, int nsize, int nns, int qual, int etype, int pscrn, bool opt, bool int16_prescreener, bool int16_predictor, int exp, bool show_mask, bool combed_only)
{
    VSMap *args, *ret;
    args = vsapi->createMap();
    vsapi->propSetNode(args, "clip", node, paReplace);
    vsapi->propSetInt(args, "field", field, paReplace);
    vsapi->propSetInt(args, "dh", dh, paReplace);
    if (!planes.empty()) {
        for (auto plane: planes) {
            vsapi->propSetInt(args, "planes", plane, paAppend);
        }
    }
    vsapi->propSetInt(args, "nsize", nsize, paReplace);
    vsapi->propSetInt(args, "nns", nns, paReplace);

    // vsapi->propSetInt(args, "qual", qual, paReplace);//error ????

    vsapi->propSetInt(args, "etype", etype, paReplace);
    vsapi->propSetInt(args, "pscrn", pscrn, paReplace);
    vsapi->propSetInt(args, "opt", opt, paReplace);
    vsapi->propSetInt(args, "int16_prescreener", int16_prescreener, paReplace);
    vsapi->propSetInt(args, "int16_predictor", int16_predictor, paReplace);
    vsapi->propSetInt(args, "exp", exp, paReplace);
    vsapi->propSetInt(args, "show_mask", show_mask, paReplace);
    vsapi->propSetInt(args, "combed_only", combed_only, paReplace);
    ret = vsapi->invoke(nnedi3Plugin, "nnedi3", args);
    //getMap(vsapi, ret);
    VSNodeRef *retnode = vsapi->propGetNode(ret, "clip", 0, nullptr);

    vsapi->freeMap(args);
    vsapi->freeMap(ret);
    return retnode;
}
