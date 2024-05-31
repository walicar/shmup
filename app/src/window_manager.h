#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#else

#include <glad.h>

#endif

#include <GLFW/glfw3.h>
#include <bitset>
#include <string>
#include "ecs/types.h"


class WindowManager {

public:
    WindowManager() = delete;

    WindowManager(std::string const &title, int width, int height);

    void clean();

    void update();

    void process_events();


private:
    GLFWwindow *window;

    std::bitset<8> buttons;

    void set_buttons(GLint key_code, InputButtons input_code);
};


#endif