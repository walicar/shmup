#include <glad.h>
#include <glfw3.h>
#include <iostream>
#include "shader.h"
#include "resource_manager.h"
#include "window_manager.h"
#include "ecs/coordinator.h"
#include "ecs/types.h"
#include "components/sprite.h"
#include "systems/sprite_system.h"
#include <chrono>

Coordinator GCR;
static bool quit = false;

void handle_quit(Event &e) {
    quit = true;
}

int main() {
    // application entry
    WindowManager window_manager("SHMUP", 800, 600);
    GCR.add_listener(FUNCTION_LISTENER(Events::Window::QUIT, handle_quit)); // lambda preferred?

    Shader def_shader = ResourceManager::load_shader("shaders/default.vert", "shaders/default.frag", "default");
    Texture def_texture = ResourceManager::load_texture("textures/smile.png", false, "smile");

    float v[] = {
            -0.5f, -0.5f, 0.0f,     0.5, 0.0,       // bottom left
            0.5f, -0.5f, 0.0f,      1.0, 0.5,       // bottom right
            0.0f, 0.5f, 0.0f,       0.5, 1.0        // upper middle
    };

    GCR.register_component<Sprite>();
    auto sprite_system = GCR.register_system<SpriteSystem>();

    std::vector<Entity> entities(MAX_ENTITIES - 1);

    entities[0] = GCR.create_entity();
    GCR.add_component(entities[0], Sprite{
        .shader = &def_shader,
        .texture = &def_texture,
        .vertex_data = v,
        .vertex_count = 15 // @TODO: HARDCODED
    });


    auto& sprite = GCR.get_component<Sprite>(entities[0]);
    sprite.setup();

    float dt = 0.0f;

    while (!quit) {
        // auto start = std::chrono::high_resolution_clock::now();
        window_manager.process_events();
        // scuffed
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //
        sprite_system->update((float)glfwGetTime()); // glfwGetTime() returns negative numbers??
        window_manager.update();
        auto stop = std::chrono::high_resolution_clock::now();
        // dt = std::chrono::duration<float, std::chrono::seconds::period>(stop - start).count();

    }
    // window_manager should deallocate here?
    window_manager.clean();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
