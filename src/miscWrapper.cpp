/*================================================================
*
*   File:		miscWrapper.cpp
*   Author:		Ding Binyuang(dbinyuan@163.com)
*   Date:		2021-05-11  15:37
*   Version:		1.0
*   Description:
*
================================================================*/

#include "miscWrapper.h"
#include "common.h"

VSNodeRef* MiscWrapper::SCDetect(VSNodeRef* node, double threshold)
{
    VSMap *args, *ret;
    args = vsapi->createMap();
    vsapi->propSetNode(args, "clip", node, paReplace);
    vsapi->propSetFloat(args, "threshold", threshold, paReplace);

    ret = vsapi->invoke(miscPlugin, "SCDetect", args);
    DEBUG_SHOW(0);

    VSNodeRef *retnode = vsapi->propGetNode(ret, "clip", 0, nullptr);

    vsapi->freeMap(args);
    vsapi->freeMap(ret);
    return retnode;
}

VSNodeRef* MiscWrapper::averageFrames(VSNodeRef* node, std::vector<double> weights, int scenechange, std::vector<int> planes)
{
    VSMap *args, *ret;
    args = vsapi->createMap();

    vsapi->propSetNode(args, "clips", node, paReplace);

    vsapi->propSetInt(args, "scenechange", scenechange, paReplace);
    if (!weights.empty()) {
        for (auto weight: weights) {
            vsapi->propSetFloat(args, "weights", weight, paAppend);
        }
    }
    if (!planes.empty()) {
        for (auto plane: planes) {
            vsapi->propSetInt(args, "planes", plane, paAppend);
        }
    }

    ret = vsapi->invoke(miscPlugin, "AverageFrames", args);
    DEBUG_SHOW(0);

    VSNodeRef *retnode = vsapi->propGetNode(ret, "clip", 0, nullptr);

    vsapi->freeMap(args);
    vsapi->freeMap(ret);
    return retnode;
}

