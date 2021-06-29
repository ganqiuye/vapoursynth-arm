/*================================================================
*
*   File:		common.cpp
*   Author:		Gan Qiuye(ganqiuye@163.com)
*   Date:		2021-05-10  14:57
*   Version:		1.0
*   Description:
*
================================================================*/

#include "common.h"
#include <dlfcn.h>
#include <string.h>
#include <malloc.h>

void testDlopen()
{
    void *handle = dlopen("/data/vapoursynth/libmvtools.so", RTLD_LAZY);
    printf("=====handle:%p\n", handle);
    if(handle == NULL){
        fprintf(stderr,"%s\n",dlerror());
        return;
    }
    dlclose(handle);
}

int getMap(const VSAPI* vsapi, VSMap *map)
{
    int nums = vsapi->propNumKeys(map);
    printf("map Nums:%d\n", nums);
    int err;
    for(int i=0; i<nums; i++)
    {
        const char* key   = vsapi->propGetKey(map, i);
        const char *value = vsapi->propGetData(map, key, 0, &err);
        printf("    key: %s\t, value: %s\n", key, value);
    }
    return nums;
}

int getPluginFuncs(const VSAPI* vsapi, VSPlugin* plugin)
{
    VSMap *map = vsapi->getFunctions(plugin);
    int funcNums = getMap(vsapi, map);
    vsapi->freeMap(map);
    return funcNums;
}

void show_plugins(VSCore *core, const VSAPI *vsapi)
{
    VSMap *m = vsapi->getPlugins(core);
    VSMap *n;

    int i;
    int numkeys = vsapi->propNumKeys(m);
    for (i = 0; i < numkeys; i++)
    {
	const char *a_tmp = vsapi->propGetData(m, vsapi->propGetKey(m, i), 0, NULL);
	char *a = (char *)malloc(strlen(a_tmp));
	strncpy(a, a_tmp, strlen(a_tmp));
	LOG("%s", a);

	const char *d = ";";
	char *b = strtok(a, d);
	int str_num = 0;
	while (b)
	{
	    b = strtok(NULL, d);
	    str_num ++;
	    if (str_num == 1) break;
	}
	//LOG("%s", b);

	n = vsapi->getFunctions(vsapi->getPluginById(b, core));
	int j;
	int numkeys_j = vsapi->propNumKeys(n);
	LOG("funcs num: %d", numkeys_j);
	for (j = 0; j < numkeys_j; j++)
	{
	    //const char *c_tmp = vsapi->propGetData(n, vsapi->propGetKey(n, j), 0, NULL);
	    //LOG("%s", c_tmp);
	}
	free(a);
	vsapi->freeMap(n);
    }

    vsapi->freeMap(m);
}

void show_map_elements(const VSAPI *vsapi, VSMap *map)
{
    int i, j;
    int num_keys = vsapi->propNumKeys(map);
    for (i = 0; i < num_keys; i++)
    {
	const char *retkey = vsapi->propGetKey(map, i);
	char proptype = vsapi->propGetType(map, retkey);
	int num_elements = vsapi->propNumElements(map, retkey);

	for (j = 0; j < num_elements; j++)
	{
	    if (proptype == 'i')
	    {
		printf("map type int -> num: %d, key: %s\n", j, retkey);
	    }
	    else if (proptype == 'f')
	    {
		printf("map type float -> num: %d, key: %s\n", j, retkey);
	    }
	    else if (proptype == 's')
	    {
		printf("map type char* -> num: %d, key: %s\n", j, retkey);
	    }
	    else if (proptype == 'c')
	    {
		printf("map type node -> num: %d, key: %s\n", j, retkey);
	    }
	    else if (proptype == 'v')
	    {
		printf("map type frame -> num: %d, key: %s\n", j, retkey);
	    }
	    else if (proptype == 'm')
	    {
		printf("map type func -> num: %d, key: %s\n", j, retkey);
	    }
	}
    }
}

