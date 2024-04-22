#include <glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include "shader.h"
#include "resource_manager.h"
#include "ecs/coordinator.h"
#include "ecs/entity.h"
#include "components/sprite.h"
#include "systems/sprite_system.h"
#include <iostream>
#include <filesystem>


void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);

Coordinator GC9R;

int main() {
    // application entry
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required for MacOS

    GLFWwindow *window = glfwCreateWindow(800, 600, "SHMUP", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader def_shader = ResourceManager::load_shader("shaders/default.vert", "shaders/default.frag", "default");
    Texture def_texture = ResourceManager::load_texture("textures/smile.png", false, "smile");

    float v[] = {
            -0.5f, -0.5f, 0.0f,     0.5, 0.0,       // bottom left
            0.5f, -0.5f, 0.0f,      1.0, 0.5,       // bottom right
            0.0f, 0.5f, 0.0f,       0.5, 1.0        // upper middle
    };

    GC9R.register_component<Sprite>();
    auto sprite_system = GC9R.register_system<SpriteSystem>();

    std::vector<Entity> entities(MAX_ENTITIES - 1);

    entities[0] = GC9R.create_entity();
    GC9R.add_component(entities[0], Sprite{
        .shader = &def_shader,
        .texture = &def_texture,
        .vertex_data = v,
        .vertex_count = 15 // @TODO: HARDCODED
    });


    auto& sprite = GC9R.get_component<Sprite>(entities[0]);
    sprite.setup();

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        sprite_system->update((float)glfwGetTime());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window)
{
}
