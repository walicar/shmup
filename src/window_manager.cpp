#include "window_manager.h"
#include "ecs/coordinator.h"

#include <bitset>
#include <iostream>


extern Coordinator GCR;


WindowManager::WindowManager(std::string const &title, unsigned int width, unsigned int height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required for MacOS

    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
    }

    int f_width, f_height;
    glfwGetFramebufferSize(window, &f_width, &f_height);

    glViewport(0, 0, f_width, f_height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // glfwSwapInterval(2);
}

void WindowManager::clean() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void WindowManager::update() {
    glfwSwapBuffers(window);
}

void WindowManager::set_buttons(GLint key_code, InputButtons input_code) {
    if (glfwGetKey(window, key_code) == GLFW_PRESS) {
        buttons.set(static_cast<std::size_t>(input_code));
    } else if (glfwGetKey(window, key_code) == GLFW_RELEASE) {
        buttons.reset(static_cast<std::size_t>(input_code));
    }
}

void WindowManager::process_events() {
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        GCR.send_event(Events::Window::QUIT);
    }

    set_buttons(GLFW_KEY_W, InputButtons::W);
    set_buttons(GLFW_KEY_A, InputButtons::A);
    set_buttons(GLFW_KEY_S, InputButtons::S);
    set_buttons(GLFW_KEY_D, InputButtons::D);
    set_buttons(GLFW_KEY_J, InputButtons::J);
    set_buttons(GLFW_KEY_K, InputButtons::K);
    set_buttons(GLFW_KEY_L, InputButtons::L);
    set_buttons(GLFW_KEY_LEFT_SHIFT, InputButtons::SHIFT);

    Event event(Events::Window::INPUT);
    event.set_param(Events::Window::Input::INPUT, buttons);
    GCR.send_event(event);
}
