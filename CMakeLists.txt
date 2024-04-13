cmake_minimum_required(VERSION 3.24)
project(code)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_OSX_ARCHITECTURES, "arm64")

include_directories(.)
include_directories(inc)
include_directories(inc/glm)
include_directories(lib)
include_directories(glad)

find_library(GLFW_LIB glfw)
link_libraries(${GLFW_LIB})

add_executable(code
        lib/glad.c
        src/main.cpp
        src/shader.cpp
        src/resource_manager.cpp
)

target_link_libraries(code "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")