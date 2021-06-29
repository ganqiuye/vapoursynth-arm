/*================================================================
*
*   File:		stdWrapper.h
*   Author:		Bill Cong(cjcbill@gmail.com)
*   Date:		2021-05-12  16:28
*   Version:		1.0
*   Description:
*
================================================================*/
#ifndef __STD_WRAPPER_H__
#define __STD_WRAPPER_H__

#include <vector>
#include "VapourSynth.h"
#include "basePluginWrapper.h"

struct StdWrapper : public BasePluginWrapper
{
    StdWrapper(const VSAPI* vsapi, VSCore *core):
      BasePluginWrapper(vsapi, core, "com.vapoursynth.std") {
        stdPlugin = getPlugin();
      }
    virtual ~StdWrapper() {}
    /*simplefilters.c*/
    VSNodeRef* separateFields(VSNodeRef* node, bool tff);
    VSNodeRef* doubleWeave(VSNodeRef* node, bool tff);
    VSNodeRef* blankClip(VSNodeRef* node, std::vector<float> colors);

    /*mergefilters.c*/
    VSNodeRef* merge(VSNodeRef* clipa,
                     VSNodeRef* clipb,
                     std::vector<float> weights);

    VSNodeRef* makeDiff(VSNodeRef* clipa,
                        VSNodeRef* clipb,
                        std::vector<int> planes);

    VSNodeRef* mergeDiff(VSNodeRef* clipa,
                         VSNodeRef* clipb,
                         std::vector<int> planes);

    VSNodeRef* maskedMerge(VSNodeRef* clipa,
                          VSNodeRef* clipb,
                          VSNodeRef* mask);

    /*genericfilters.cpp*/
    VSNodeRef* minimum(VSNodeRef* node,
                       std::vector<int> planes,
                       std::vector<int> coordinates);

    VSNodeRef* maximum(VSNodeRef* node,
                       std::vector<int> planes,
                       std::vector<int> coordinates);

    VSNodeRef* median(VSNodeRef* node,
                      std::vector<int> planes);

    VSNodeRef* deflate(VSNodeRef* node,
                       std::vector<int> planes);

    VSNodeRef* inflate(VSNodeRef* node,
                       std::vector<int> planes);

    VSNodeRef* convolution(VSNodeRef* node,
                           std::vector<float> matrix,
                           std::vector<int> planes);

    /*exprfilter.cpp*/
    VSNodeRef* expr(std::vector<VSNodeRef*> nodes,
                    std::vector<const std::string> exprs,
                    int format = -1);

private:
    VSNodeRef* simpleCommonFunc(const std::string& name,
                                VSNodeRef* node,
                                bool tff);

    VSNodeRef* mergeCommonFunc(const std::string& name,
                               VSNodeRef* clipa,
                               VSNodeRef* clipb,
                               std::vector<float> weights,
                               std::vector<int> planes);

    VSNodeRef* genericCommonFunc(const std::string& name,
                                 VSNodeRef* node,
                                 std::vector<int> planes,
                                 std::vector<int> coordinates);
    VSPlugin* stdPlugin;
    DISALLOW_COPY_AND_ASSIGN(StdWrapper);
};

#endif //__STD_WRAPPER_H__
