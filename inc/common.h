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
   File:		common.h
   Author:		Gan Qiuye(ganqiuye@163.com)
   Date:		2021-05-10  14:57
   Version:		1.0
   Description:
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

/* 查询所有插件，及函数个数 */
void show_plugins(VSCore *core, const VSAPI *vsapi);

/* 查询 VSMap 的所有键，方便明确 invoke 函数调用后所返回的值是什么 */
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

