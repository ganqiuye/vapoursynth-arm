AUTOMAKE_OPTIONS=foreign
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

testva_CXXFLAGS = -O3 -g -fPIC -fPIE

LOCAL_INCLUDE = -I./inc \
        -I./platform/android/vapoursynth/include/vapoursynth

testva_LDFLAGS =

testva_CXXFLAGS += $(LOCAL_INCLUDE)

LOCAL_LIB = -L./platform/android/vapoursynth/lib -lvapoursynth
testva_LDFLAGS += $(LOCAL_LIB)