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
#include "components/projectile.h"
#include "components/tags/controllable.h"
#include "components/transform.h"
#include "components/ai.h"
#include "systems/sprite_system.h"
#include "systems/movement_system.h"
#include "systems/projectile_system.h"
#include "systems/physics_system.h"
#include "systems/collision_system.h"
#include "systems/ai_system.h"
#include "systems/animation_system.h"
#include "src/components/state.h"
#include "src/systems/spawn_system.h"
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
    // Texture def_texture = ResourceManager::load_texture("textures/smile.png", "smile", true);
    Texture laser_texture = ResourceManager::load_texture("textures/laser.png", "laser", true);
    Texture bomb_texture = ResourceManager::load_texture("textures/bomb.png", "bomb", true);
    Texture bullet_texture = ResourceManager::load_texture("textures/bullet.png", "bullet", true);
    Texture ship_texture = ResourceManager::load_texture("textures/ship.png", "ship", true);
    Texture ebullet_texture = ResourceManager::load_texture("textures/ebullet.png", "ebullet", true);
    Texture eship_texture = ResourceManager::load_texture("textures/eship.png", "eship", true);
    Texture snipe_texture = ResourceManager::load_texture("textures/snipe.png", "snipe", true);
    Texture star_texture = ResourceManager::load_texture("textures/star.png", "star", true);
    Texture hose_texture = ResourceManager::load_texture("textures/hose.png", "hose", true);
    // Texture boss_texture = ResourceManager::load_texture("textures/snipe.png", "snipe", true);

    float v[] = {
            -0.25f, -0.5f, 0.0f, 0.0f, 0.0f,       // bottom left
            0.25f, -0.5f, 0.0f, 1.0f, 0.0f,       // bottom right
            0.0f, 0.5f, 0.0f, 0.5f, 1.0f        // upper middle
    };

    float lv[] = {
            -0.25f, -1.0f, 0.0f, 0.0f, 0.0f,
            0.25f, -1.0f, 0.0f, 1.0f, 0.0f,
            0.25f, 1.0f, 0.0f, 1.0f, 1.0f,
            -0.25f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    float bv[] = {
            -0.25f, -0.5f, 0.0f, 0.0f, 0.0f,
            0.25f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.25f, 0.5f, 0.0f, 1.0f, 1.0f,
            -0.25f, 0.5f, 0.0f, 0.0f, 1.0f
    };

    float ev[] = {
            -0.25f, 0.5f, 0.0f, 0.0, 0.0,
            0.25f, 0.5f, 0.0f, 1.0, 0.0,
            0.0f, -0.5f, 0.0f, 0.5, 1.0
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
    GCR.register_component<AI>();
    GCR.register_component<State>();

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

    auto projectile_system = GCR.register_system<ProjectileSystem>();
    {
        Signature signature;
        signature.set(GCR.get_component_type<Player>());
        signature.set(GCR.get_component_type<Sprite>());
        signature.set(GCR.get_component_type<Projectile>());
        GCR.set_system_signature<ProjectileSystem>(signature);
    }

    projectile_system->init();

    auto collision_system = GCR.register_system<CollisionSystem>();
    {
        Signature signature;
        signature.set(GCR.get_component_type<Hitbox>());
        signature.set(GCR.get_component_type<Sprite>());
        signature.set(GCR.get_component_type<Transform>());
        GCR.set_system_signature<CollisionSystem>(signature);
    }

    collision_system->init();

    auto physics_system = GCR.register_system<PhysicsSystem>();
    {
        Signature signature;
        signature.set(GCR.get_component_type<Transform>());
        signature.set(GCR.get_component_type<Velocity>());
        GCR.set_system_signature<PhysicsSystem>(signature);
    }

    physics_system->init();

    auto ai_system = GCR.register_system<AISystem>();
    {
        Signature signature;
        signature.set(GCR.get_component_type<Transform>());
        signature.set(GCR.get_component_type<Enemy>());
        signature.set(GCR.get_component_type<Sprite>());
        GCR.set_system_signature<AISystem>(signature);
    }

    ai_system->init();

    auto animation_system = GCR.register_system<AnimationSystem>();
    {
        Signature signature;
        signature.set(GCR.get_component_type<Sprite>());
        GCR.set_system_signature<AnimationSystem>(signature);
    }

    animation_system->init();

    auto spawn_system = GCR.register_system<SpawnSystem>();
    {
        Signature signature;
        signature.set(GCR.get_component_type<Enemy>());
        GCR.set_system_signature<SpawnSystem>(signature);
    }

    spawn_system->init();

    // create the player
    Entity player = GCR.create_entity();
    GCR.add_component(player, State{
        .active = true
    });
    GCR.add_component(player, Sprite{
            .shader = &def_shader,
            .texture = &ship_texture,
            .vertex_data = v,
            .vertex_count = 3 // @TODO: HARDCODED
    });
    GCR.add_component(player, Transform{});
    GCR.add_component(player, Player{});
    GCR.add_component(player, Controllable{});
    GCR.add_component(player, Hitbox{
            .hitbox = glm::vec3(0.05f, 0.05f, 0.05f)
    });
    auto &sprite = GCR.get_component<Sprite>(player);
    sprite.setup();

    // player projectiles

    // laser...
    Entity player_laser = GCR.create_entity();
    GCR.add_component(player_laser, State{});
    GCR.add_component(player_laser, Sprite{
            .shader = &def_shader,
            .texture = &laser_texture,
            .vertex_data = lv,
            .vertex_count = 4 // @TODO: HARDCODED
    });

    GCR.add_component(player_laser, Transform{
            .pos = glm::vec3(10.0f, 0.0f, 0.0f)
    });
    GCR.add_component(player_laser, Hitbox{
            .hitbox = glm::vec3(0.5f, 1.5f, 1.0f)
    });
    GCR.add_component(player_laser, Velocity{});
    GCR.add_component(player_laser, Player{});
    GCR.add_component(player_laser, Projectile{
        .damage = 100 // debug damage, set back to 1
    });
    auto &laser_sprite = GCR.get_component<Sprite>(player_laser);
    laser_sprite.setup();

    // bombs
    for (int i = 0; i < Entities::P_BOMB_AMT; i++) {
        Entity player_bomb = GCR.create_entity();
        GCR.add_component(player_bomb, State{});
        GCR.add_component(player_bomb, Sprite{
                .shader = &def_shader,
                .texture = &bomb_texture,
                .vertex_data = bv,
                .vertex_count = 4 // @TODO: HARDCODED
        });
        GCR.add_component(player_bomb, Transform{});
        GCR.add_component(player_bomb, Projectile{
            .type = BOMB,
            .damage = 10
        });
        GCR.add_component(player_bomb, Player{});
        GCR.add_component(player_bomb, Hitbox{
                .hitbox = glm::vec3(100.0f)
        });
        auto &bullet_sprite = GCR.get_component<Sprite>(player_bomb);
        bullet_sprite.setup();
    }

    // bullets
    for (int i = 0; i < Entities::P_BULLET_AMT; i++) {
        Entity player_bullet = GCR.create_entity();
        GCR.add_component(player_bullet, State{
                .active = false // redundant
        });
        GCR.add_component(player_bullet, Sprite{
                .shader = &def_shader,
                .texture = &bullet_texture,
                .vertex_data = bv,
                .vertex_count = 4 // @TODO: HARDCODED
        });

        GCR.add_component(player_bullet, Transform{});
        GCR.add_component(player_bullet, Player{});
        GCR.add_component(player_bullet, Hitbox{
                .hitbox = glm::vec3(0.5f, 0.5f, 1.0f)
        });
        GCR.add_component(player_bullet, Velocity{});
        GCR.add_component(player_bullet, Projectile{
            .damage = 15
        });
        auto &bullet_sprite = GCR.get_component<Sprite>(player_bullet);
        bullet_sprite.setup();
    }

    // enemies

    // enemy grunts
    for (int i = 0; i < Entities::E_AMT; i++) {
        Entity enemy = GCR.create_entity();
        GCR.add_component(enemy, State{
                .active = true,
        });
        GCR.add_component(enemy, Sprite{
                .shader = &def_shader,
                .texture = &eship_texture,
                .vertex_data = ev,
                .vertex_count = 3 // @TODO: HARDCODED
        });
        GCR.add_component(enemy, Transform{
                .pos = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
                .origin = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
        });
        GCR.add_component(enemy, Enemy{
            .type = GRUNT
        });
        GCR.add_component(enemy, AI{
                .attack_cooldown = 2.0f,
                .last_attacked = 0.0f + (1.0f * i)
        });
        GCR.add_component(enemy, Hitbox{
                .hitbox = glm::vec3(0.5f, 0.5f, 0.5f)
        });
        auto &enemy_sprite = GCR.get_component<Sprite>(enemy);
        enemy_sprite.setup();
    }

    // snipe
    for (int i = 0; i < Entities::E_AMT; i++) {
        Entity enemy = GCR.create_entity();
        GCR.add_component(enemy, State{
                .active = false,
        });
        GCR.add_component(enemy, Sprite{
                .shader = &def_shader,
                .texture = &snipe_texture,
                .vertex_data = ev,
                .vertex_count = 3 // @TODO: HARDCODED
        });
        GCR.add_component(enemy, Transform{
                .pos = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
                .origin = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
        });
        GCR.add_component(enemy, Enemy{
            .type = SNIPE
        });
        GCR.add_component(enemy, AI{
                .attack_cooldown = 3.0f,
                .last_attacked = 0.0f + (1.0f * i)
        });
        GCR.add_component(enemy, Hitbox{
                .hitbox = glm::vec3(0.5f, 0.5f, 0.5f)
        });
        auto &enemy_sprite = GCR.get_component<Sprite>(enemy);
        enemy_sprite.setup();
    }

    // hose
    for (int i = 0; i < Entities::E_AMT; i++) {
        Entity enemy = GCR.create_entity();
        GCR.add_component(enemy, State{
                .active = false,
        });
        GCR.add_component(enemy, Sprite{
                .shader = &def_shader,
                .texture = &hose_texture,
                .vertex_data = ev,
                .vertex_count = 3 // @TODO: HARDCODED
        });
        GCR.add_component(enemy, Transform{
                .pos = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
                .origin = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
        });
        GCR.add_component(enemy, Enemy{
                .type = HOSE
        });
        GCR.add_component(enemy, AI{
                .attack_cooldown = 0.75f,
                .last_attacked = 0.0f + (1.25f * i)
        });
        GCR.add_component(enemy, Hitbox{
            .health = 50,
            .hitbox = glm::vec3(0.5f, 0.5f, 0.5f)
        });
        auto &enemy_sprite = GCR.get_component<Sprite>(enemy);
        enemy_sprite.setup();
    }

    // star
    for (int i = 0; i < Entities::E_AMT; i++) {
        Entity enemy = GCR.create_entity();
        GCR.add_component(enemy, State{
                .active = false,
        });
        GCR.add_component(enemy, Sprite{
                .shader = &def_shader,
                .texture = &star_texture,
                .vertex_data = ev,
                .vertex_count = 3 // @TODO: HARDCODED
        });
        GCR.add_component(enemy, Transform{
                .pos = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
                .origin = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
        });
        GCR.add_component(enemy, Enemy{
                .type = STAR
        });
        GCR.add_component(enemy, AI{
                .attack_cooldown = 1.0f,
                .last_attacked = 0.0f + (0.25f * i)
        });
        GCR.add_component(enemy, Hitbox{
                .health = 50,
                .hitbox = glm::vec3(0.5f, 0.5f, 0.5f)
        });
        auto &enemy_sprite = GCR.get_component<Sprite>(enemy);
        enemy_sprite.setup();
    }

    for (int i = 0; i < Entities::E_AMT; i++) {
        if (i % 3 == 0) {
            Entity enemy = GCR.create_entity();
            GCR.add_component(enemy, State{
                    .active = false,
            });
            GCR.add_component(enemy, Sprite{
                    .shader = &def_shader,
                    .texture = &snipe_texture,
                    .vertex_data = ev,
                    .vertex_count = 3 // @TODO: HARDCODED
            });
            GCR.add_component(enemy, Transform{
                    .pos = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
                    .origin = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
            });
            GCR.add_component(enemy, Enemy{
                    .type = SNIPE
            });
            GCR.add_component(enemy, AI{
                    .attack_cooldown = 3.0f,
                    .last_attacked = 0.0f + (1.0f * i)
            });
            GCR.add_component(enemy, Hitbox{
                    .hitbox = glm::vec3(0.5f, 0.5f, 0.5f)
            });
            auto &enemy_sprite = GCR.get_component<Sprite>(enemy);
            enemy_sprite.setup();
        } else if (i % 3 == 1) {


            Entity enemy = GCR.create_entity();
            GCR.add_component(enemy, State{
                    .active = false,
            });
            GCR.add_component(enemy, Sprite{
                    .shader = &def_shader,
                    .texture = &hose_texture,
                    .vertex_data = ev,
                    .vertex_count = 3 // @TODO: HARDCODED
            });
            GCR.add_component(enemy, Transform{
                    .pos = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
                    .origin = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
            });
            GCR.add_component(enemy, Enemy{
                    .type = HOSE
            });
            GCR.add_component(enemy, AI{
                    .attack_cooldown = 0.75f,
                    .last_attacked = 0.0f + (1.25f * i)
            });
            GCR.add_component(enemy, Hitbox{
                    .health = 50,
                    .hitbox = glm::vec3(0.5f, 0.5f, 0.5f)
            });
            auto &enemy_sprite = GCR.get_component<Sprite>(enemy);
            enemy_sprite.setup();
        } else {
            Entity enemy = GCR.create_entity();
        GCR.add_component(enemy, State{
                .active = false,
        });
        GCR.add_component(enemy, Sprite{
                .shader = &def_shader,
                .texture = &star_texture,
                .vertex_data = ev,
                .vertex_count = 3 // @TODO: HARDCODED
        });
        GCR.add_component(enemy, Transform{
                .pos = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
                .origin = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
        });
        GCR.add_component(enemy, Enemy{
                .type = STAR
        });
        GCR.add_component(enemy, AI{
                .attack_cooldown = 1.0f,
                .last_attacked = 0.0f + (0.25f * i)
        });
        GCR.add_component(enemy, Hitbox{
                .health = 50,
                .hitbox = glm::vec3(0.5f, 0.5f, 0.5f)
        });
        auto &enemy_sprite = GCR.get_component<Sprite>(enemy);
        enemy_sprite.setup();
        }
    }

    // boss
    Entity boss = GCR.create_entity();
    std::cout << "BOSS ID ::: " << boss << std::endl;

    // enemy bullets
    for (int i = 0; i < Entities::E_BULLET_AMT; i++) {
        Entity enemy_bullet = GCR.create_entity();
        GCR.add_component(enemy_bullet, State{
                .active = false,
        });
        GCR.add_component(enemy_bullet, Sprite{
                .shader = &def_shader,
                .texture = &ebullet_texture,
                .vertex_data = bv,
                .vertex_count = 4 // @TODO: HARDCODED
        });

        GCR.add_component(enemy_bullet, Transform{
            .pos = glm::vec3(0.0f),
            .origin = glm::vec3(0.0f)
        });
        GCR.add_component(enemy_bullet, Enemy{});
        GCR.add_component(enemy_bullet, Hitbox{
                .hitbox = glm::vec3(0.5f, 0.5f, 1.0f)
        });
        GCR.add_component(enemy_bullet, Velocity{});
        GCR.add_component(enemy_bullet, Projectile{});
        auto &bullet_sprite = GCR.get_component<Sprite>(enemy_bullet);
        bullet_sprite.setup();
    }

    float dt = 0.0f;

    while (!quit) {
        auto start = (float) glfwGetTime();
        window_manager.process_events();
        projectile_system->update((float) glfwGetTime());
        movement_system->update(dt);
        ai_system->update((float) glfwGetTime());
        collision_system->update(dt);
        physics_system->update(dt);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        sprite_system->update((float) glfwGetTime());
        animation_system->update((float) glfwGetTime());
        spawn_system->update();
        window_manager.update();
        auto stop = (float) glfwGetTime();
        dt = stop - start;

    }
    window_manager.clean();
    return 0;
}
