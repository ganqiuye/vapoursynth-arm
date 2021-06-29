/*================================================================
 *
 *   File:		stdWrapper.cpp
 *   Author:	Bill Cong(cjcbill@gmail.com)
 *   Date:		2021-05-13  10:30
 *   Version:		1.0
 *   Description:
 *
 ================================================================*/

#include <string>
#include "stdWrapper.h"
#include "common.h"

VSNodeRef* StdWrapper::separateFields(VSNodeRef* node, bool tff)
{
    return simpleCommonFunc(std::string("SeparateFields"), node, tff);
}

VSNodeRef* StdWrapper::doubleWeave(VSNodeRef* node, bool tff)
{
    return simpleCommonFunc(std::string("DoubleWeave"), node, tff);
}

VSNodeRef* StdWrapper::blankClip(VSNodeRef* node, std::vector<float> colors)
{
    VSMap *args, *ret;
    args = vsapi->createMap();
    vsapi->propSetNode(args, "clip", node, paReplace);
    for (auto color : colors) {
      vsapi->propSetFloat(args, "color", color, paAppend);
    }

    ret = vsapi->invoke(stdPlugin, "BlankClip", args);

    DEBUG_SHOW(0);

    VSNodeRef *retnode = vsapi->propGetNode(ret, "clip", 0, nullptr);
    vsapi->freeMap(args);
    vsapi->freeMap(ret);
    return retnode;
}

VSNodeRef* StdWrapper::merge(VSNodeRef* clipa,
        VSNodeRef* clipb,
        std::vector<float> weights)
{
    return mergeCommonFunc(std::string("Merge"),
                           clipa,
                           clipb,
                           weights,
                           std::vector<int>());
}

VSNodeRef* StdWrapper::makeDiff(VSNodeRef* clipa,
                                VSNodeRef* clipb,
                                std::vector<int> planes)
{
    return mergeCommonFunc(std::string("MakeDiff"),
                           clipa,
                           clipb,
                           std::vector<float>(),
                           planes);
}

VSNodeRef* StdWrapper::mergeDiff(VSNodeRef* clipa,
                                 VSNodeRef* clipb,
                                 std::vector<int> planes)
{
    return mergeCommonFunc(std::string("MergeDiff"),
                           clipa,
                           clipb,
                           std::vector<float>(),
                           planes);
}

VSNodeRef* StdWrapper::maskedMerge(VSNodeRef* clipa,
                                  VSNodeRef* clipb,
                                  VSNodeRef* mask)
{
    VSMap *args, *ret;
    args = vsapi->createMap();
    vsapi->propSetNode(args, "clipa", clipa, paReplace);
    vsapi->propSetNode(args, "clipb", clipb, paReplace);
    vsapi->propSetNode(args, "mask", mask, paReplace);

    ret = vsapi->invoke(stdPlugin, "MaskedMerge", args);

    DEBUG_SHOW(0);

    VSNodeRef *retnode = vsapi->propGetNode(ret, "clip", 0, nullptr);
    vsapi->freeMap(args);
    vsapi->freeMap(ret);
    return retnode;

}

VSNodeRef* StdWrapper::minimum(VSNodeRef* node,
                               std::vector<int> planes,
                               std::vector<int> coordinates)
{
    return genericCommonFunc(std::string("Minimum"), node, planes, coordinates);
}

VSNodeRef* StdWrapper::maximum(VSNodeRef* node,
                               std::vector<int> planes,
                               std::vector<int> coordinates)
{
    return genericCommonFunc(std::string("Maximum"), node, planes, coordinates);
}


VSNodeRef* StdWrapper::median(VSNodeRef* node,
                              std::vector<int> planes)
{
    return genericCommonFunc(std::string("Median"),
            node,
            planes,
            std::vector<int>());
}


VSNodeRef* StdWrapper::deflate(VSNodeRef* node,
                               std::vector<int> planes)
{
    return genericCommonFunc(std::string("Deflate"),
                             node,
                             planes,
                             std::vector<int>());
}

VSNodeRef* StdWrapper::inflate(VSNodeRef* node,
                               std::vector<int> planes)
{
    return genericCommonFunc(std::string("Inflate"),
                             node,
                             planes,
                             std::vector<int>());
}

VSNodeRef* StdWrapper::convolution(VSNodeRef* node,
                                   std::vector<float> matrix,
                                   std::vector<int> planes)
{
    VSMap *args, *ret;
    args = vsapi->createMap();
    vsapi->propSetNode(args, "clip", node, paReplace);

    for (auto item: matrix) {
        vsapi->propSetFloat(args, "matrix", item, paAppend);
    }

    for (auto plane: planes) {
        vsapi->propSetInt(args, "planes", plane, paAppend);
    }

    ret = vsapi->invoke(stdPlugin, "Convolution", args);

    DEBUG_SHOW(0);

    VSNodeRef *retnode = vsapi->propGetNode(ret, "clip", 0, nullptr);
    vsapi->freeMap(args);
    vsapi->freeMap(ret);
    return retnode;
}



VSNodeRef* StdWrapper::expr(std::vector<VSNodeRef*> nodes,
                            std::vector<const std::string> exprs,
                            int format)
{
    VSMap *args, *ret;
    args = vsapi->createMap();

    for (auto node: nodes)
    {
        vsapi->propSetNode(args, "clips", node, paAppend);
    }

    for (auto expr: exprs)
    {
        vsapi->propSetData(args, "expr", expr.c_str(), expr.length(), paAppend);
    }

    if (format)
    {
        vsapi->propSetInt(args, "format", format, paReplace);
    }

    ret = vsapi->invoke(stdPlugin, "Expr", args);

    DEBUG_SHOW(0);
    VSNodeRef *retnode = vsapi->propGetNode(ret, "clip", 0, nullptr);
    vsapi->freeMap(args);
    vsapi->freeMap(ret);
    return retnode;
}


VSNodeRef* StdWrapper::simpleCommonFunc(const std::string& name,
        VSNodeRef* node,
        bool tff)
{
    VSMap *args, *ret;
    args = vsapi->createMap();
    vsapi->propSetNode(args, "clip", node, paReplace);
    vsapi->propSetInt(args, "tff", tff, paReplace);
    ret = vsapi->invoke(stdPlugin, name.c_str(), args);

    DEBUG_SHOW(0);

    VSNodeRef *retnode = vsapi->propGetNode(ret, "clip", 0, nullptr);
    vsapi->freeMap(args);
    vsapi->freeMap(ret);
    return retnode;
}

VSNodeRef* StdWrapper::mergeCommonFunc(const std::string& name,
        VSNodeRef* clipa,
        VSNodeRef* clipb,
        std::vector<float> weights,
        std::vector<int> planes)
{
    VSMap *args, *ret;
    args = vsapi->createMap();
    vsapi->propSetNode(args, "clipa", clipa, paReplace);
    vsapi->propSetNode(args, "clipb", clipb, paReplace);

    if (!weights.empty()) {
        for (auto weight: weights) {
            vsapi->propSetFloat(args, "weight", weight, paAppend);
        }
    }

    if (!planes.empty()) {
        for (auto plane: planes) {
            vsapi->propSetInt(args, "planes", plane, paAppend);
        }
    }

    ret = vsapi->invoke(stdPlugin, name.c_str(), args);

    DEBUG_SHOW(0);


    VSNodeRef *retnode = vsapi->propGetNode(ret, "clip", 0, nullptr);
    vsapi->freeMap(args);
    vsapi->freeMap(ret);
    return retnode;
}



VSNodeRef* StdWrapper::genericCommonFunc(const std::string& name,
        VSNodeRef* node,
        std::vector<int> planes,
        std::vector<int> coordinates)
{
    VSMap *args, *ret;
    args = vsapi->createMap();
    vsapi->propSetNode(args, "clip", node, paReplace);
    for (auto plane: planes) {
        vsapi->propSetInt(args, "planes", plane, paAppend);
    }

    if (!coordinates.empty()) {
        for (auto coordinate: coordinates) {
            vsapi->propSetInt(args, "coordinates", coordinate, paAppend);
        }
    }

    ret = vsapi->invoke(stdPlugin, name.c_str(), args);

    DEBUG_SHOW(0);


    VSNodeRef *retnode = vsapi->propGetNode(ret, "clip", 0, nullptr);
    vsapi->freeMap(args);
    vsapi->freeMap(ret);
    return retnode;
}
