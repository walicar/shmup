cmake_minimum_required(VERSION 3.24)
project(code)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_OSX_ARCHITECTURES, "arm64")

include_directories(.)
include_directories(Inc)
include_directories(Inc/GL)
include_directories(Inc/GLFW)
include_directories(Inc/glm)
include_directories(Lib)
include_directories(glad)

find_library(GLFW_LIB glfw)
link_libraries(${GLFW_LIB})

add_executable(code
        Lib/glad.c
        Src/main.cpp)

target_link_libraries(code "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")