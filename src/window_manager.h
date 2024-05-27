#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <glad.h>
#include <glfw3.h>
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