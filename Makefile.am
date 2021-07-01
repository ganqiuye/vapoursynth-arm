AUTOMAKE_OPTIONS=foreign
warning_flags = -Wall -Wextra -Wno-unused-parameter -Wshadow
common_cflags = -O2 $(MFLAGS) $(STACKREALIGN) $(warning_flags)
AM_CFLAGS = -std=c99 $(common_cflags)
AM_CXXFLAGS = -std=c++11 $(common_cflags)
AM_CPPFLAGS = $(VapourSynth_CFLAGS)

bin_PROGRAMS = testva

testva_SOURCES =  ./src/main.cpp \
                  ./src/basePluginWrapper.cpp \
                  ./src/QTGMC.cpp \
                  ./src/common.cpp \
                  ./src/miscWrapper.cpp \
                  ./src/removeGrainWrapper.cpp \
                  ./src/ffms2Wrapper.cpp \
                  ./src/mvtoolsWrapper.cpp \
                  ./src/stdWrapper.cpp \
                  ./src/fmtcWrapper.cpp \
                  ./src/nnedi3Wrapper.cpp \
                  ./src/textWrapper.cpp

LOCAL_INCLUDE = -I./inc

testva_CXXFLAGS = -fPIC -fPIE $(LOCAL_INCLUDE)

testva_LDFLAGS = -no-undefined -avoid-version $(PLUGINLDFLAGS)

testva_LDADD = $(VapourSynth_LIBS)
