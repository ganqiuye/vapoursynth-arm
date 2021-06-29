CXX := arm-linux-androideabi-g++

INCLUDES := -I./vapoursynth/include/vapoursynth \
            -I./inc

LIBS := -L./vapoursynth/lib -lvapoursynth

SRCDIRS := ./src
SRCCS := $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.cpp))

OBJ := $(SRCCS:%.cpp=%.o)

CXXFLAGS := -O3 -g -fPIC -fPIE

CXXFLAGS += $(INCLUDES)

TARGET = testva

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^  $(LIBS)

clean:
	@rm -f $(TARGET)
	@rm -f $(OBJ)
