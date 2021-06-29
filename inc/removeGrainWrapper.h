/*================================================================
 *
 *   File:	        removeGrainWrapper.h
 *   Author:		Bill Cong(cjcbill@gmail.com)
 *   Date:		2021-05-13  09:13
 *   Version:		1.0
 *   Description:
 *
 ================================================================*/
#ifndef __REMOVEGRAIN_WRAPPER_H__
#define __REMOVEGRAIN_WRAPPER_H__

#include "VapourSynth.h"
#include "basePluginWrapper.h"

struct RemoveGrainWrapper : public BasePluginWrapper
{
    RemoveGrainWrapper(const VSAPI* vsapi, VSCore *core):
        BasePluginWrapper(vsapi, core, "com.vapoursynth.removegrainvs")
    {
        removeGrainPlugin = getPlugin();
    }
    virtual ~RemoveGrainWrapper() {}

    VSNodeRef* repair(VSNodeRef* clip,
                      VSNodeRef* repairclip,
                      std::vector<int> modes);

private:
    VSPlugin* removeGrainPlugin;
    DISALLOW_COPY_AND_ASSIGN(RemoveGrainWrapper);
};

#endif //__REMOVEGRAIN_WRAPPER_H__
