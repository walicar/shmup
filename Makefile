CXX = em++
CXX_TEST = g++

WEB_DIR = web/assets
INC_DIR = app/inc
LIB_DIR = app/lib
SRC_DIR = app/src
SHADERS_DIR = app/src/shaders/es/
TEXTURES_DIR = app/src/textures/
FONTS_DIR = app/src/fonts/

SRCS = $(shell find $(SRC_DIR) $(LIB_DIR) -name '*.cpp')
OBJS = $(SRCS:.cpp=.o)

INCLUDE_DIRS = $(shell find $(INC_DIR) -type d)
INCLUDE_FLAGS = $(addprefix -I, $(INCLUDE_DIRS))
EMS_FLAGS = -sUSE_FREETYPE=1
CPP_FLAGS = -std=c++20 $(INCLUDE_FLAGS)
PACKAGING = --embed-file $(FONTS_DIR)@fonts/ --embed-file $(SHADERS_DIR)@shaders/ --embed-file $(TEXTURES_DIR)@textures/
LD_FLAGS = -sMAX_WEBGL_VERSION=2 -s USE_GLFW=3 -s FULL_ES3=1 -s ALLOW_MEMORY_GROWTH=1 -s NO_EXIT_RUNTIME=0 -s ASSERTIONS=1 $(PACKAGING) -lfreetype

%.o: %.cpp
	$(CXX) $(CPP_FLAGS) $(EMS_FLAGS) -c -o $@ $<

main: $(OBJS) $(WEB_DIR)
	$(CXX) -o $(WEB_DIR)/out.js $(OBJS) $(LD_FLAGS)

emhtml: $(OBJS) $(WEB_DIR)
	$(CXX) -o tests/index.html $(OBJS) $(LD_FLAGS)

clean:
	rm -rf web/assets/out* tests/index* $(SRCS:.cpp=.o) *.o

.PHONY: clean