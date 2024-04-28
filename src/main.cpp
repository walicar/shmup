#include <glad.h>
#include <glfw3.h>
#include <iostream>
#include "shader.h"
#include "resource_manager.h"
#include "window_manager.h"
#include "ecs/coordinator.h"
#include "ecs/types.h"
#include "components/sprite.h"
#include "components/velocity.h"
#include "components/hitbox.h"
#include "components/tags/player.h"
#include "components/tags/enemy.h"
#include "components/tags/projectile.h"
#include "components/tags/controllable.h"
#include "components/transform.h"
#include "systems/sprite_system.h"
#include "systems/movement_system.h"
#include "systems/projectile_system.h"
#include "systems/collision_system.h"
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
    Texture laser_texture = ResourceManager::load_texture("textures/laser.png", false, "laser");

    float v[] = {
            -0.25f, -0.5f, 0.0f, 0.5, 0.0,       // bottom left
            0.25f, -0.5f, 0.0f, 1.0, 0.5,       // bottom right
            0.0f, 0.5f, 0.0f, 0.5, 1.0        // upper middle
    };

    float lv[] = {
            -0.25f, -1.0f, 0.0f, 0.0f, 0.0f,
            0.25f, -1.0f, 0.0f, 1.0f, 0.0f,
            0.25f, 1.0f, 0.0f, 1.0f, 1.0f,
            -0.25f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    // register components
    GCR.register_component<Sprite>();
    GCR.register_component<Transform>();
    GCR.register_component<Hitbox>();
    GCR.register_component<Velocity>();
    GCR.register_component<Player>();
    GCR.register_component<Enemy>();
    GCR.register_component<Projectile>();
    GCR.register_component<Controllable>();

    // register systems
    /**
     * NOTE: any entity that has all components required by a system
     *  will be added to the that system's entity list
     */
    auto sprite_system = GCR.register_system<SpriteSystem>();
    {
        Signature signature;
        signature.set(GCR.get_component_type<Transform>());
        signature.set(GCR.get_component_type<Sprite>());
        GCR.set_system_signature<SpriteSystem>(signature);
    }

    auto movement_system = GCR.register_system<MovementSystem>(); // rename to movement system
    {
        Signature signature;
        signature.set(GCR.get_component_type<Transform>());
        signature.set(GCR.get_component_type<Player>());
        signature.set(GCR.get_component_type<Controllable>());
        signature.set(GCR.get_component_type<Hitbox>());
        GCR.set_system_signature<MovementSystem>(signature);
    }

    movement_system->init();

    // ProjectileSystem will need to listen to key events like control system
    auto projectile_system = GCR.register_system<ProjectileSystem>();
    {
        Signature signature;
        signature.set(GCR.get_component_type<Player>());
        // @TODO: not sure if we edit Sprite or Projectile
        signature.set(GCR.get_component_type<Sprite>());
        signature.set(GCR.get_component_type<Projectile>());
        GCR.set_system_signature<MovementSystem>(signature);
    }

    projectile_system->init();

    // CollisionSystem will need to figure out what is colliding?
    // - need to check enemy colliding with player
    // - need to check player colliding with enemy
    auto collision_system = GCR.register_system<CollisionSystem>();
    {
        Signature signature;
        signature.set(GCR.get_component_type<Hitbox>());
        signature.set(GCR.get_component_type<Sprite>());
        signature.set(GCR.get_component_type<Transform>());
        GCR.set_system_signature<CollisionSystem>(signature);
    }

    collision_system->init();

    // create the player
    Entity player = GCR.create_entity();
    GCR.add_component(player, Sprite{
        .shader = &def_shader,
        .texture = &def_texture,
        .scale_factor = glm::vec3(0.25f),
        .active = true,
        .vertex_data = v,
        .vertex_count = 15 // @TODO: HARDCODED
    });
    GCR.add_component(player, Transform{});
    GCR.add_component(player, Player{});
    GCR.add_component(player, Controllable{});
    GCR.add_component(player, Hitbox{
        .hitbox = glm::vec3(0.5f, 0.5f, 0.5f)
    });
    auto& sprite = GCR.get_component<Sprite>(player);
    sprite.setup();

    // player projectiles

    // laser...
    Entity player_laser = GCR.create_entity();
    GCR.add_component(player_laser, Sprite {
        .shader = &def_shader,
        .texture = &laser_texture,
        .active = false,
        .vertex_data = lv,
        .vertex_count = 20 // @TODO: HARDCODED
    });

    GCR.add_component(player_laser, Transform{});
    GCR.add_component(player_laser, Hitbox{
        .hitbox = glm::vec3(0.5f, 1.5f, 1.0f)
    });
    GCR.add_component(player_laser, Velocity{});
    GCR.add_component(player_laser, Player{});
    GCR.add_component(player_laser, Projectile{});
    auto& laser_sprite = GCR.get_component<Sprite>(player_laser);
    laser_sprite.setup();

    // enemies
    Entity enemy = GCR.create_entity();
    GCR.add_component(enemy, Sprite{
            .shader = &def_shader,
            .texture = &def_texture,
            .scale_factor = glm::vec3(0.25f),
            .active = true,
            .vertex_data = v,
            .vertex_count = 15 // @TODO: HARDCODED
    });

    GCR.add_component(enemy, Transform{});
    GCR.add_component(enemy, Enemy{});
    GCR.add_component(enemy, Hitbox{
        .hitbox = glm::vec3(0.5f, 0.5f, 0.5f)
    });
    auto& enemy_sprite = GCR.get_component<Sprite>(enemy);
    enemy_sprite.setup();

    float dt = 0.0f;

    while (!quit) {
        auto start = (float)glfwGetTime();
        window_manager.process_events();
        movement_system->update(dt);
        projectile_system->update(dt);
        collision_system->update(dt);
        // move this
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //
        sprite_system->update((float)glfwGetTime()); // glfwGetTime() returns negative numbers??
        window_manager.update();
        auto stop = (float)glfwGetTime();
        dt = stop - start;

    }
    // window_manager should deallocate here?
    window_manager.clean();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
