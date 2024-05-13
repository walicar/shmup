cmake_minimum_required(VERSION 3.24)
project(SHMUP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_OSX_ARCHITECTURES, "arm64")

find_package(Freetype REQUIRED)

include_directories(.)
include_directories(inc)
include_directories(inc/glm)
include_directories(lib)
include_directories(glad)
include_directories(${FREETYPE_INCLUDE_DIRS})

find_library(GLFW_LIB glfw)
link_libraries(${GLFW_LIB})
link_libraries(${FREETYPE_LIBRARIES})

add_executable(code
        lib/glad.c
        lib/stb_image.cpp
        src/main.cpp
        src/shader.cpp
        src/resource_manager.cpp
        src/texture.cpp
        src/utils.cpp
        src/systems/sprite_system.cpp
        src/window_manager.cpp
        src/systems/movement_system.cpp
        src/systems/projectile_system.cpp
        src/systems/collision_system.cpp
        src/systems/physics_system.cpp
        src/systems/ai_system.cpp
        src/systems/animation_system.cpp
        src/systems/spawn_system.cpp
        src/systems/text_system.cpp
)

target_link_libraries(code "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")