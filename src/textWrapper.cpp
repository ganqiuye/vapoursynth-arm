/*================================================================
*
*   File:		textWrapper.cpp
*   Author:		Ding Binyuang(dbinyuan@163.com)
*   Date:		2021-05-14  13:40
*   Version:		1.0
*   Description:
*
================================================================*/

#include "textWrapper.h"
#include "common.h"

VSNodeRef* TextWrapper::Text(VSNodeRef* node, const char *text, int64_t alignment, int64_t scale)
{
    VSMap *args, *ret;
    args = vsapi->createMap();
    vsapi->propSetNode(args, "clip", node, paReplace);
    vsapi->propSetData(args, "text", text, -1, paReplace);
    vsapi->propSetInt(args, "alignment", alignment, paReplace);
    vsapi->propSetInt(args, "scale", scale, paReplace);

    ret = vsapi->invoke(textPlugin, "Text", args);

    DEBUG_SHOW(0);

    VSNodeRef *retnode = vsapi->propGetNode(ret, "clip", 0, nullptr);

    vsapi->freeMap(args);
    vsapi->freeMap(ret);
    return retnode;
}

