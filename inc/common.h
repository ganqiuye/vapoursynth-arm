/*================================================================
*
*   File:		common.h
*   Author:		Gan Qiuye(ganqiuye@163.com)
*   Date:		2021-05-10  14:57
*   Version:		1.0
*   Description:
*
================================================================*/
#ifndef __COMMON_H__
#define __COMMON_H__
#include "VapourSynth.h"
#include <stdio.h>

#define DISALLOW_COPY_AND_ASSIGN(ClassName) \
    ClassName(const ClassName&); \
    void operator=(const ClassName&);

#define LOG(fmt, ...)	printf("[%s->%d]:" fmt "\n", __func__, __LINE__, ##__VA_ARGS__)

#define NONE 0x1fffffff

void testDlopen();

int getMap(const VSAPI* vsapi, VSMap *map);

int getPluginFuncs(const VSAPI* vsapi, VSPlugin* plugin);

/* ��ѯ���в�������������� */
void show_plugins(VSCore *core, const VSAPI *vsapi);

/* ��ѯ VSMap �����м���������ȷ invoke �������ú������ص�ֵ��ʲô */
void show_map_elements(const VSAPI *vsapi, VSMap *map);

#define DEBUG_SHOW(show) \
    do{  \
        if(show) \
        { \
            LOG("\t*** show map elements     ***");\
            show_map_elements(vsapi, ret); \
            LOG("\t*** show map elements end ***"); \
        }\
    }while(0);
#endif//__COMMON_H__

