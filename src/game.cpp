#include "game.h"

#include "sprite_cache.h"
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
#include "components/state.h"

extern Coordinator GCR;

// just DI instead
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Game::Game() {
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
    sprite_system = GCR.register_system<SpriteSystem>();
    {
        Signature signature;
        signature.set(GCR.get_component_type<Transform>());
        signature.set(GCR.get_component_type<Sprite>());
        GCR.set_system_signature<SpriteSystem>(signature);
    }

    movement_system = GCR.register_system<MovementSystem>(); // rename to movement system
    {
        Signature signature;
        signature.set(GCR.get_component_type<Transform>());
        signature.set(GCR.get_component_type<Player>());
        signature.set(GCR.get_component_type<Controllable>());
        signature.set(GCR.get_component_type<Hitbox>());
        GCR.set_system_signature<MovementSystem>(signature);
    }

    movement_system->init();

    projectile_system = GCR.register_system<ProjectileSystem>();
    {
        Signature signature;
        signature.set(GCR.get_component_type<Player>());
        signature.set(GCR.get_component_type<Sprite>());
        signature.set(GCR.get_component_type<Projectile>());
        GCR.set_system_signature<ProjectileSystem>(signature);
    }

    projectile_system->init();

    collision_system = GCR.register_system<CollisionSystem>();
    {
        Signature signature;
        signature.set(GCR.get_component_type<Hitbox>());
        signature.set(GCR.get_component_type<Sprite>());
        signature.set(GCR.get_component_type<Transform>());
        GCR.set_system_signature<CollisionSystem>(signature);
    }

    collision_system->init();

    physics_system = GCR.register_system<PhysicsSystem>();
    {
        Signature signature;
        signature.set(GCR.get_component_type<Transform>());
        signature.set(GCR.get_component_type<Velocity>());
        GCR.set_system_signature<PhysicsSystem>(signature);
    }

    physics_system->init();

    ai_system = GCR.register_system<AISystem>();
    {
        Signature signature;
        signature.set(GCR.get_component_type<Transform>());
        signature.set(GCR.get_component_type<Enemy>());
        signature.set(GCR.get_component_type<Sprite>());
        GCR.set_system_signature<AISystem>(signature);
    }

    ai_system->init();

    animation_system = GCR.register_system<AnimationSystem>();
    {
        Signature signature;
        signature.set(GCR.get_component_type<Sprite>());
        GCR.set_system_signature<AnimationSystem>(signature);
    }

    animation_system->init();

    spawn_system = GCR.register_system<SpawnSystem>();
    {
        Signature signature;
        signature.set(GCR.get_component_type<Enemy>());
        GCR.set_system_signature<SpawnSystem>(signature);
    }

    spawn_system->init();

    // Create Entities
    Sprite player_sprite = SpriteCache::get_sprite("player");
    Sprite player_laser_sprite = SpriteCache::get_sprite("plaser");
    Sprite player_bomb_sprite = SpriteCache::get_sprite("pbomb");
    Sprite player_bullet_sprite = SpriteCache::get_sprite("pbullet");
    //
    Sprite enemy_bullet_sprite = SpriteCache::get_sprite("ebullet");
    Sprite grunt_sprite = SpriteCache::get_sprite("grunt");
    Sprite snipe_sprite = SpriteCache::get_sprite("snipe");
    Sprite hose_sprite = SpriteCache::get_sprite("hose");
    Sprite star_sprite = SpriteCache::get_sprite("star");
    Sprite boss_sprite = SpriteCache::get_sprite("boss");

    // create the player
    Entity player = GCR.create_entity();
    GCR.add_component(player, State{
            .active = true
    });
    GCR.add_component(player, player_sprite);
    GCR.add_component(player, Transform{});
    GCR.add_component(player, Player{});
    GCR.add_component(player, Controllable{});
    GCR.add_component(player, Hitbox{
            .health = 300,
            .hitbox = glm::vec3(0.05f, 0.05f, 0.05f)
    });

    // player projectiles

    // laser...
    Entity player_laser = GCR.create_entity();
    GCR.add_component(player_laser, State{});
    GCR.add_component(player_laser, player_laser_sprite);

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

    // bombs
    for (int i = 0; i < Entities::P_BOMB_AMT; i++) {
        Entity player_bomb = GCR.create_entity();
        GCR.add_component(player_bomb, State{});
        GCR.add_component(player_bomb, player_bomb_sprite);
        GCR.add_component(player_bomb, Transform{});
        GCR.add_component(player_bomb, Projectile{
                .type = BOMB,
        });
        GCR.add_component(player_bomb, Player{});
        GCR.add_component(player_bomb, Hitbox{
                .hitbox = glm::vec3(100.0f)
        });
    }

    // bullets
    for (int i = 0; i < Entities::P_BULLET_AMT; i++) {
        Entity player_bullet = GCR.create_entity();
        GCR.add_component(player_bullet, State{
                .active = false // redundant
        });
        GCR.add_component(player_bullet, player_bullet_sprite);

        GCR.add_component(player_bullet, Transform{});
        GCR.add_component(player_bullet, Player{});
        GCR.add_component(player_bullet, Hitbox{
                .hitbox = glm::vec3(0.5f, 0.5f, 1.0f)
        });
        GCR.add_component(player_bullet, Velocity{});
        GCR.add_component(player_bullet, Projectile{
                .damage = 15
        });
    }

    // player core, it's the visual representation of the hitbox
    Entity player_core = GCR.create_entity();
    GCR.add_component(player_core, State{
            .active = false
    });
    GCR.add_component(player_core, player_sprite);
    GCR.add_component(player_core, Player{});
    GCR.add_component(player_core, Transform{});
    auto &player_core_sprite = GCR.get_component<Sprite>(player_core);
    player_core_sprite.setup();

    // enemies

    // enemy grunts
    for (int i = 0; i < Entities::E_AMT; i++) {
        Entity enemy = GCR.create_entity();
        printf("creating GRUNT [%d]\n", enemy);
        GCR.add_component(enemy, State{
                .active = true,
        });
        GCR.add_component(enemy, grunt_sprite);
        GCR.add_component(enemy, Transform{
                .pos = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
                .origin = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
        });
        GCR.add_component(enemy, Enemy{
                .type = GRUNT
        });
        GCR.add_component(enemy, AI{
                .attack_cooldown = 1.0f,
                .last_attacked = ((i % 3) * 0.25f)
        });
        GCR.add_component(enemy, Hitbox{
                .hitbox = glm::vec3(0.5f, 0.5f, 0.5f)
        });
    }

    // snipe
    for (int i = 0; i < Entities::E_AMT; i++) {
        printf("creating SNIPE [%d]\n", i);
        Entity enemy = GCR.create_entity();
        GCR.add_component(enemy, State{
                .active = false,
        });
        GCR.add_component(enemy, snipe_sprite);
        GCR.add_component(enemy, Transform{
                .pos = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
                .origin = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
        });
        GCR.add_component(enemy, Enemy{
                .type = SNIPE
        });
        GCR.add_component(enemy, AI{
                .attack_cooldown = 1.0f,
                .last_attacked = 0.0f + (1.0f * i)
        });
        GCR.add_component(enemy, Hitbox{
                .hitbox = glm::vec3(0.5f, 0.5f, 0.5f)
        });
    }

    // hose
    for (int i = 0; i < Entities::E_AMT; i++) {
        Entity enemy = GCR.create_entity();
        printf("creating HOSE [%d]\n", enemy);
        GCR.add_component(enemy, State{
                .active = false,
        });
        GCR.add_component(enemy, hose_sprite);
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
    }

    // star
    for (int i = 0; i < Entities::E_AMT; i++) {
        Entity enemy = GCR.create_entity();
        printf("creating STAR [%d]\n", enemy);
        GCR.add_component(enemy, State{
                .active = false,
        });
        GCR.add_component(enemy, star_sprite);
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
    }

    for (int i = 0; i < Entities::E_AMT; i++) {
        Entity enemy = GCR.create_entity();
        if (i % 3 == 0) {
            printf("creating SNIPE [%d]\n", enemy);
            GCR.add_component(enemy, State{
                    .active = false,
            });
            GCR.add_component(enemy, snipe_sprite);
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
        } else if (i % 3 == 1) {
            printf("creating HOSE [%d]\n", enemy);
            GCR.add_component(enemy, State{
                    .active = false,
            });
            GCR.add_component(enemy, hose_sprite);
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
        } else {
            printf("creating STAR [%d]\n", enemy);
            GCR.add_component(enemy, State{
                    .active = false,
            });
            GCR.add_component(enemy, hose_sprite);
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
        }
    }

    // boss
    Entity boss = GCR.create_entity();
    GCR.add_component(boss, State{
            .active = false,
    });
    boss_sprite.scale_factor = glm::vec3(0.22f);
    GCR.add_component(boss, boss_sprite);
    GCR.add_component(boss, Transform{
            .pos = glm::vec3(0.0f, 3.5f, 0.0f),
            .origin = glm::vec3(0.0f, 4.0f, 0.0f),
    });
    GCR.add_component(boss, Enemy{
            .type = BOSS
    });
    GCR.add_component(boss, AI{
            .attack_cooldown = 0.17f,
    });
    GCR.add_component(boss, Hitbox{
            .health = 800,
            .hitbox = glm::vec3(1.75f)
    });

    // enemy bullets
    for (int i = 0; i < Entities::E_BULLET_AMT; i++) {
        Entity enemy_bullet = GCR.create_entity();
        GCR.add_component(enemy_bullet, State{
                .active = false,
        });
        GCR.add_component(enemy_bullet, enemy_bullet_sprite);

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
    }

}

void Game::loop(float dt) {
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
}

void Game::reset() {
}

