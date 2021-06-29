# vapoursynth-arm使用说明

## 1. 前言

vapoursynth-arm是基于开源项目vapoursynth开发的，旨在**将vapoursynth移植到arm（android)平台上运行。**

开发的难点在于vapoursynth项目依赖许多`cypthon, python`的代码，这对于arm平台不适用。所以，我们重新定制了一套框架来对接vapoursynth与各插件的联系。

## 2. 准备工作

### 2.1 交叉编译工具链

在实例开发中，环境是ubuntu, 目标板是android机器

所以需要准备交叉编译工具链，由于vapoursynth和大多插件都使用c++14，所以这里的交叉编译工具链是基于`android-ndk-r18b-linux-x86_64.zip`生成的。

## 2.2 下载源码

在本工程中，主要实现`QTGMC`的功能，那么需要下载以下开源代码：

* zimg(vapoursynth依赖)
* vapoursynth(自带许多插件)
* ffmpeg（ffms2依赖）
* ffms2（解析视频源）
* vapoursynth-nnedi3（di处理模块）
* fftw-3.3.9(vapoursynth-mvtools依赖)
* vapoursynth-mvtools（运动矢量相关）
* fmtconv（格式转换相关）

## 3. 开源源码编译

编译时，基本都是同一个套路：

1. `./autogen.sh`
2. `./configure`
3. `make `
4. `make install`

主要是在第2步，因为需要配置许多条件，每个插件都有不同的编译条件，可以通过`./configure -h`命令查看相关的条件

假设，交叉编译工具链在：

```
/home/A/toolchain/android_toolchain
```

<font color=red>开始前，在终端运行一下命令：</font>

```shell
export PATH=/home/A/toolchain/android_toolchain/bin:%PATH
```

### 3.1 zimg

1. `./autogen.sh`
2. `./configure --prefix=/home/A/vapoursynth_out/zimg --host=armlinux-androideabi`
3. `make && make install`

编译出：

```
.
├── include
├── lib
└── share
```

### 3.2 vapoursynth

1. 修改源码

   ```diff
   diff --git a/src/core/vscore.cpp b/src/core/vscore.cpp
   old mode 100644
   new mode 100755
   index 2d29844d..9767a14b
   --- a/src/core/vscore.cpp
   +++ b/src/core/vscore.cpp
   @@ -30,6 +30,7 @@
    #endif
    #include <cassert>
    #include <queue>
   +#include <iostream>
   
    #ifdef VS_TARGET_CPU_X86
    #include "x86utils.h"
   @@ -1147,7 +1148,6 @@ bool VSCore::loadAllPluginsInPath(const std::string &path, const std::string &fi
        DIR *dir = opendir(path.c_str());
        if (!dir)
            return false;
   -
        int name_max = pathconf(path.c_str(), _PC_NAME_MAX);
        if (name_max == -1)
            name_max = 255;
   @@ -1164,6 +1164,7 @@ bool VSCore::loadAllPluginsInPath(const std::string &path, const std::string &fi
                try {
                    std::string fullname;
                    fullname.append(path).append("/").append(name);
   +                std::cout << "====loadplugin==: " << fullname << "\n";
                    loadPlugin(fullname);
                } catch (VSException &) {
                    // Ignore any errors
   @@ -1347,7 +1348,11 @@ VSCore::VSCore(int threads) :
        if (xdg_config_home) {
            configFile.append(xdg_config_home).append("/vapoursynth/vapoursynth.conf");
        } else if (home) {
   +#if __ANDROID__
   +        configFile = "/etc/vapoursynth.conf";
   +#else
            configFile.append(home).append("/.config/vapoursynth/vapoursynth.conf");
   +#endif
        } // If neither exists, an empty string will do.
    #endif
   +    std::cout << "configFile: " << configFile << "\n";
   
   ```

   打上以上patch，这个需要指定插件的安装位置，在android中配置文件`vapoursynth.conf`放到了`/etc`目录下。配置文件的内容如下：

   ```
   UserPluginDir=/data/vapoursynth
   SystemPluginDir=/system/lib/vapoursynth
   ```


2. `./autogen.sh`

3. configure配置

   ```shell
   ./configure --prefix=/home/A/vapoursynth_out/vapoursynth --host=arm-linux-androideabi ZIMG_CFLAGS=-I/home/A/vapoursynth_out/zimg/include ZIMG_LIBS="-L/home/A/vapoursynth_out/zimg/lib -lzimg"  --enable-subtext=no --enable-python-module=no --with-sysroot=/home/A/toolchain/android_toolchain/sysroot --enable-vsscript=no --enable-vspipe=no
   ```

4. `make && make install`

5. 编译出

```
.
├── include
└── lib
```

### 3.3 vapoursynth-nnedi3

1. 修改源码

  ```diff
diff --git a/src/cpufeatures.cpp b/src/cpufeatures.cpp
index 3fcee4c..c67535f 100644
--- a/src/cpufeatures.cpp
+++ b/src/cpufeatures.cpp
@@ -70,17 +70,17 @@ void getCPUFeatures(CPUFeatures *cpuFeatures) {
 void getCPUFeatures(CPUFeatures *cpuFeatures) {
     memset(cpuFeatures, 0, sizeof(CPUFeatures));
 
-    unsigned long long hwcap = getauxval(AT_HWCAP);
+    // unsigned long long hwcap = getauxval(AT_HWCAP);
 
     cpuFeatures->can_run_vs = 1;
 
 #if defined(NNEDI3_ARM)
-    cpuFeatures->half_fp = !!(hwcap & HWCAP_ARM_HALF);
-    cpuFeatures->edsp = !!(hwcap & HWCAP_ARM_EDSP);
-    cpuFeatures->iwmmxt = !!(hwcap & HWCAP_ARM_IWMMXT);
-    cpuFeatures->neon = !!(hwcap & HWCAP_ARM_NEON);
-    cpuFeatures->fast_mult = !!(hwcap & HWCAP_ARM_FAST_MULT);
-    cpuFeatures->idiv_a = !!(hwcap & HWCAP_ARM_IDIVA);
+    // cpuFeatures->half_fp = !!(hwcap & HWCAP_ARM_HALF);
+    // cpuFeatures->edsp = !!(hwcap & HWCAP_ARM_EDSP);
+    // cpuFeatures->iwmmxt = !!(hwcap & HWCAP_ARM_IWMMXT);
+    // cpuFeatures->neon = !!(hwcap & HWCAP_ARM_NEON);
+    // cpuFeatures->fast_mult = !!(hwcap & HWCAP_ARM_FAST_MULT);
+    // cpuFeatures->idiv_a = !!(hwcap & HWCAP_ARM_IDIVA);
 #elif defined(NNEDI3_PPC)
     cpuFeatures->altivec = !!(hwcap & PPC_FEATURE_HAS_ALTIVEC);
     cpuFeatures->spe = !!(hwcap & PPC_FEATURE_HAS_SPE);
  ```

2. `./autogen.sh`

3. configure配置

```shell
./configure --prefix=/home/A/vapoursynth_out/nnedi3 --host=arm-linux-androideabi VapourSynth_CFLAGS="-I/home/A/vapoursynth_out/vapoursynth/include/vapoursynth" VapourSynth_LIBS="-L/home/A/vapoursynth_out/vapoursynth/lib -lvapoursynth" --with-sysroot="/home/A/toolchain/android_toolchain/sysroot" CPPFLAGS="-I/home/A/toolchain/android_toolchain/sysroot/usr/include"
```

4. `make && make install`

编译出：

```
.
├── include
└── lib
```

### 3.4 ffmpeg

1. 

```shell
	./configure \
	--prefix=/home/A/vapoursynth_out/ffmpeg \
	--disable-doc \
	--enable-shared \
	--disable-ffplay \
	--disable-ffprobe \
	--disable-avdevice \
	--cross-prefix=arm-linux-androideabi- \
	--target-os=android \
	--arch=arm \
	--enable-cross-compile \
	--extra-cflags="-Os -fpic -fPIE -pie -DANDROID -marm" \
	--extra-ldflags="-fPIE -pie"
```

2. `make && make install`

编译出：

```
.
├── bin
├── include
├── lib
└── share
```

### 3.5 ffms2

1. `./autogen.sh`

2.  

   ```shell
   ./configure --prefix=/home/A/vapoursynth_out/ffms --host=arm-linux-androideabi \
   FFMPEG_CFLAGS="-I/home/A/vapoursynth_out/ffmpeg/include" \
   FFMPEG_LIBS="-L/home/A/vapoursynth_out/ffmpeg/lib -lavcodec -lavformat -lswresample -lavfilter -lavutil -lswscale" \
   --with-sysroot="/home/A/toolchain/android_toolchain/sysroot" CXXFLAGS="-D_LIBCPP_HAS_NO_OFF_T_FUNCTIONS" --enable-shared=yes
   
   ```

3. `make && make install`

编译出：

```
├── bin
├── include
├── lib
└── share
```

### 3.6 fftw-3.3.9

1. 

```shell
./configure --prefix=/home/A/vapoursynth_out/fftw3out --host=arm-linux-androideabi --with-sysroot=/home/A/toolchain/android_toolchain/sysroot --enable-shared=yes --enable-neon --enable-float LIBS="-lc -lgcc" --enable-threads
```

2. `make && make install`

编译出：

```
├── bin
├── include
├── lib
└── share
```

### 3.7 vapoursynth-mvtools

1. `./autogen.sh`

2.  

   ```shell
   ./configure --prefix=/home/A/vapoursynth_out/mvtoolsout --host=arm-linux-androideabi --with-sysroot=/home/A/toolchain/android_toolchain/sysroot VapourSynth_CFLAGS="-I/home/A/vapoursynth_out/vapoursynth/include/vapoursynth" VapourSynth_LIBS="-L/home/A/vapoursynth_out/vapoursynth/lib -lvapoursynth" FFTW3F_CFLAGS="-I/home/A/vapoursynth_out/fftw3out/include" FFTW3F_LIBS="-L/home/A/vapoursynth_out/fftw3out/lib -lfftw3f"
   ```

3. `make && make install`

编译出：

```
├── lib
```

### 3.8 fmtconv

1. `cd fmtconv/build/unix`

2. `./autogen.sh`

3.  

   ```shell
   ./configure --prefix=/home/A/workspace/vaout/android/fftw3out --host=arm-linux-androideabi --with-sysroot=/home/A/workspace/toolchain/android_18/sysroot
   ```

4. `make && make install`

编译出：

```
├── lib
```

## 4. 放置生成的文件

### 4.1 工程目录

```
.
├── doc
├── download
├── inc
│   ├── basePluginWrapper.h
│   ├── common.h
│   ├── ffms2Wrapper.h
│   ├── fmtcWrapper.h
│   ├── miscWrapper.h
│   ├── mvtoolsWrapper.h
│   ├── nnedi3Wrapper.h
│   ├── QTGMC.h
│   ├── removeGrainWrapper.h
│   ├── stdWrapper.h
│   └── textWrapper.h
├── Makefile
├── platform
│   ├── config
│   ├── external
│   └── plugin
├── readme.md
├── src
│   ├── basePluginWrapper.cpp
│   ├── common.cpp
│   ├── ffms2Wrapper.cpp
│   ├── fmtcWrapper.cpp
│   ├── main.cpp
│   ├── miscWrapper.cpp
│   ├── mvtoolsWrapper.cpp
│   ├── nnedi3Wrapper.cpp
│   ├── QTGMC.cpp
│   ├── removeGrainWrapper.cpp
│   ├── stdWrapper.cpp
│   └── textWrapper.cpp
└── vapoursynth
    ├── include
    └── lib

11 directories, 25 files
```

1. `config`:放置配置文件`vapoursynth.conf`
2. `external`：放置插件依赖的库文件
3. `inc`:工程头文件
4. `plugin`：需要用到的插件库
5. `src`:工程源码
6. `vapoursynth`:vapoursynth编译出来的结果

### 4.2 目标板目录

1. `/data/vapoursynth`:放置`plugin`目录下的so库和bin文件
2. `/system/lib`:放置`vapoursynth`目录下的libvapoursynth.so库，external目录下的库
3. `/etc/`:放置`congfig`目录下的配置文件

## 5. 工程代码

```
.
├── basePluginWrapper.cpp //插件封装基类，所有插件都需要安照该接口编写
├── common.cpp        //一些通用接口
├── ffms2Wrapper.cpp  //ffm2封装
├── fmtcWrapper.cpp  //fmtc封装
├── main.cpp        //主函数
├── miscWrapper.cpp      //misc插件封装
├── mvtoolsWrapper.cpp   //mvtools插件封装
├── nnedi3Wrapper.cpp     //nnedi3插件封装
├── QTGMC.cpp             //QTGMC运行逻辑，目前是最简单的运行逻辑
├── removeGrainWrapper.cpp    //rgvs插件封装
├── stdWrapper.cpp       //std通用插件封装
└── textWrapper.cpp      //text插件封装

0 directories, 13 files
```

