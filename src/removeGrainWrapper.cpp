/*================================================================
*
*   File:		removeGrainWrapper.cpp
*   Author:		Bill Cong(cjcbill@gmail.com)
*   Date:		2021-05-14  09:30
*   Version:		1.0
*   Description:
*
================================================================*/

#include <vector>
#include "removeGrainWrapper.h"
#include "common.h"


VSNodeRef* RemoveGrainWrapper::repair(VSNodeRef* clip,
                                      VSNodeRef* repairclip,
                                      std::vector<int> modes)
{
    VSMap *args, *ret;
    args = vsapi->createMap();
    vsapi->propSetNode(args, "clip", clip, paReplace);
    vsapi->propSetNode(args, "repairclip", repairclip, paReplace);

    for (auto mode : modes)
    {
        vsapi->propSetInt(args, "mode", mode, paReplace);
    }

    ret = vsapi->invoke(removeGrainPlugin, "Repair", args);

    DEBUG_SHOW(0);

    VSNodeRef *retnode = vsapi->propGetNode(ret, "clip", 0, nullptr);
    vsapi->freeMap(args);
    vsapi->freeMap(ret);
    return retnode;
}
