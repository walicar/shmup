#include "window_manager.h"
#include "ecs/coordinator.h"

#include <bitset>
#include <iostream>


extern Coordinator GCR;


WindowManager::WindowManager(std::string const& title, unsigned int width, unsigned int height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required for MacOS

    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glViewport(0, 0, width, height);
    glfwSwapInterval(2);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void WindowManager::clean()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void WindowManager::update()
{
    glfwSwapBuffers(window);
}

void WindowManager::process_events()
{
    glfwPollEvents();

    bool buttonStateChanged = true;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE))
    {
        GCR.send_event(Events::Window::QUIT);
    }
    else if (glfwGetKey(window, GLFW_KEY_W))
    {
        buttons.set(static_cast<std::size_t>(InputButtons::W));
        std::cout << "W pressed" << std::endl;
    }
    else if (glfwGetKey(window, GLFW_KEY_A))
    {
        buttons.set(static_cast<std::size_t>(InputButtons::A));
    }
    else if (glfwGetKey(window, GLFW_KEY_S))
    {
        buttons.set(static_cast<std::size_t>(InputButtons::S));
    }
    else if (glfwGetKey(window, GLFW_KEY_D))
    {
        buttons.set(static_cast<std::size_t>(InputButtons::D));
    }
    else if (glfwGetKey(window, GLFW_KEY_J))
    {
        buttons.set(static_cast<std::size_t>(InputButtons::J));
    }
    else if (glfwGetKey(window, GLFW_KEY_K))
    {
        buttons.set(static_cast<std::size_t>(InputButtons::K));
    }
    else if (glfwGetKey(window, GLFW_KEY_L))
    {
        buttons.set(static_cast<std::size_t>(InputButtons::K));
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
    {
        buttons.set(static_cast<std::size_t>(InputButtons::SHIFT));
    }
    else
    {
        buttonStateChanged = false;
    }

    if (buttonStateChanged)
    {
        Event event(Events::Window::INPUT);
        event.set_param(Events::Window::Input::INPUT, buttons);
        GCR.send_event(event);
    }
}
