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
#include "components/tags/particle.h"
#include "components/projectile.h"
#include "components/tags/controllable.h"
#include "components/transform.h"
#include "components/ai.h"
#include "components/state.h"

enum HealthValues {
    PLAYER_HP = 300,
    GRUNT_HP = 100,
    HOSE_HP = 75,
    STAR_HP = 50,
    BOSS_HP = 800
};

extern Coordinator GCR;

Game::Game() {
    GCR.add_listener(METHOD_LISTENER(Events::Window::INPUT, Game::input));

    init_ecs();
    init_player();

    for (int i = 0; i < Entities::E_AMT; i++) { // level 0
        make_enemy(GRUNT, i);
    }
    for (int i = 0; i < Entities::E_AMT; i++) { // level 1
        make_enemy(SNIPE, i);
    }
    for (int i = 0; i < Entities::E_AMT; i++) { // level 2
        make_enemy(HOSE, i);
    }
    for (int i = 0; i < Entities::E_AMT; i++) { // level 3
        make_enemy(STAR, i);
    }
    for (int i = 0; i < Entities::E_AMT; i++) { // level 4
        if (i % 3 == 0) {
            make_enemy(SNIPE, i);
        } else if (i % 3 == 1) {
            make_enemy(HOSE, i);
        } else {
            make_enemy(STAR, i);
        }
    }
    // boss
    Sprite boss_sprite = SpriteCache::get_sprite("boss");
    Entity boss = GCR.create_entity();
    GCR.add_component(boss, State{
            .active = false,
    });
    boss_sprite.scale_factor = glm::vec3(0.17f, 0.17f, 0.10f);
    GCR.add_component(boss, boss_sprite);
    GCR.add_component(boss, Transform{
            .pos = glm::vec3(0.0f, 4.0f, 0.0f),
            .origin = glm::vec3(0.0f, 4.0f, 0.0f),
    });
    GCR.add_component(boss, Enemy{
            .type = BOSS
    });
    GCR.add_component(boss, AI{
            .attack_cooldown = 0.17f,
    });
    GCR.add_component(boss, Hitbox{
            .health = BOSS_HP,
            .hitbox = glm::vec3(1.0f)
    });

    // enemy bullets
    for (int i = 0; i < Entities::E_BULLET_AMT; i++) {
        Entity enemy_bullet = GCR.create_entity();
        GCR.add_component(enemy_bullet, State{
                .active = false,
        });
        GCR.add_component(enemy_bullet, SpriteCache::get_sprite("ebullet"));

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

void Game::update(float dt) {
    background_system->update((float) glfwGetTime());
    projectile_system->update((float) glfwGetTime());
    movement_system->update(dt);
    ai_system->update((float) glfwGetTime());
    collision_system->update(dt);
    physics_system->update(dt);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    sprite_system->update((float) glfwGetTime());
    animation_system->update((float) glfwGetTime());
    spawn_system->update();
    //
    auto player_hp = GCR.get_component<Hitbox>(Entities::PLAYER).health;
    auto boss_hp = GCR.get_component<Hitbox>(Entities::BOSS).health;
    if (player_hp <= 0 || boss_hp <= 0) {
        reset();
    }
}

void Game::start() {
    GCR.get_component<Hitbox>(Entities::PLAYER).health = PLAYER_HP;
    for (int i = 0; i < Entities::E_AMT; i++) {
        auto enemy = Entities::E_GRUNT + i;
        GCR.get_component<State>(enemy).active = true;
    }

    Event start_game(Events::Game::START);
    GCR.send_event(start_game);
    in_game = true;
}

void Game::reset() {
    in_game = false;
    Event stop_game(Events::Game::STOP);
    GCR.send_event(stop_game);
    // projectile system will listen for this event
    //  and reset bomb resources

    // reactivate player, and reset their hp
    GCR.get_component<State>(Entities::PLAYER).active = true;
    // because you can get hit with bullets after dying...
    GCR.get_component<Hitbox>(Entities::PLAYER).health = 9001;

    // put enemies in dormant state
    int enemy_amt = (Entities::E_AMT * 5);
    int checkpoint = 0;
    for (int i = 0; i < enemy_amt; i++) {
        if (i % 15 == 0)
            checkpoint += 1;
        auto enemy = Entities::E_GRUNT + i;
        GCR.get_component<State>(enemy).active = false;
        if (checkpoint < 2) {
            GCR.get_component<Hitbox>(enemy).health = GRUNT_HP;
        } else if (checkpoint == 2) {
            GCR.get_component<Hitbox>(enemy).health = HOSE_HP;
        } else if (checkpoint == 3) {
            GCR.get_component<Hitbox>(enemy).health = STAR_HP;
        } else { // checkpoint 4
            if (i % 3 == 0) {
                GCR.get_component<Hitbox>(enemy).health = GRUNT_HP;
            } else if (i % 3 == 1) {
                GCR.get_component<Hitbox>(enemy).health = HOSE_HP;
            } else {
                GCR.get_component<Hitbox>(enemy).health = STAR_HP;
            }
        }
    }

    // handle boss
    GCR.get_component<State>(Entities::BOSS).active = false;
    GCR.get_component<Hitbox>(Entities::BOSS).health = BOSS_HP;
}

void Game::input(Event &e) {
    if (!in_game) {
        auto buttons = e.get_param<std::bitset<8>>(Events::Window::Input::INPUT);
        if (buttons.test(static_cast<std::size_t>(InputButtons::SHIFT))) {
            start();
        }
    }
}

void Game::make_enemy(EnemyType type, int offset) {
    Entity enemy = GCR.create_entity();
    GCR.add_component(enemy, State{
            .active = false,
    });
    GCR.add_component(enemy, Transform{
            .pos = glm::vec3(-7.0f + (1.0f * offset), 8.0f, 0.0f),
            .origin = glm::vec3(-7.0f + (1.0f * offset), 8.0f, 0.0f),
    });
    switch (type) {
        case GRUNT:
            GCR.add_component(enemy, SpriteCache::get_sprite("grunt"));
            GCR.add_component(enemy, Enemy{
                    .type = GRUNT
            });
            GCR.add_component(enemy, AI{
                    .attack_cooldown = 1.0f,
                    .last_attacked = ((offset % 3) * 0.25f)
            });
            GCR.add_component(enemy, Hitbox{
                    .hitbox = glm::vec3(0.7f)
            });
            break;
        case SNIPE:
            GCR.add_component(enemy, SpriteCache::get_sprite("snipe"));
            GCR.add_component(enemy, Enemy{
                    .type = SNIPE
            });
            GCR.add_component(enemy, AI{
                    .attack_cooldown = 1.0f,
                    .last_attacked = 0.0f + (1.0f * offset)
            });
            GCR.add_component(enemy, Hitbox{
                    .hitbox = glm::vec3(0.7f)
            });
            break;
        case HOSE:
            GCR.add_component(enemy, SpriteCache::get_sprite("hose"));
            GCR.add_component(enemy, Enemy{
                    .type = HOSE
            });
            GCR.add_component(enemy, AI{
                    .attack_cooldown = 0.75f,
                    .last_attacked = 0.0f + (1.25f * offset)
            });
            GCR.add_component(enemy, Hitbox{
                    .health = HOSE_HP,
                    .hitbox = glm::vec3(0.7f)
            });
            break;
        case STAR:
            GCR.add_component(enemy, SpriteCache::get_sprite("star"));
            GCR.add_component(enemy, Enemy{
                    .type = STAR
            });
            GCR.add_component(enemy, AI{
                    .attack_cooldown = 1.0f,
                    .last_attacked = 0.0f + (0.25f * offset)
            });
            GCR.add_component(enemy, Hitbox{
                    .health = STAR_HP,
                    .hitbox = glm::vec3(0.7f)
            });
            break;
        case BOSS:
            // manually make boss
            break;
    }
}

void Game::init_player() {
    // create the player
    Entity player = GCR.create_entity();
    GCR.add_component(player, State{
            .active = true
    });
    GCR.add_component(player, SpriteCache::get_sprite("player"));
    GCR.add_component(player, Transform{});
    GCR.add_component(player, Player{});
    GCR.add_component(player, Controllable{});
    GCR.add_component(player, Hitbox{
            .health = PLAYER_HP,
            .hitbox = glm::vec3(0.15f)
    });

    // player projectiles

    // laser...
    Entity player_laser = GCR.create_entity();
    GCR.add_component(player_laser, State{});
    GCR.add_component(player_laser, SpriteCache::get_sprite("plaser"));

    GCR.add_component(player_laser, Transform{
            .pos = glm::vec3(10.0f, 0.0f, 0.0f)
    });
    GCR.add_component(player_laser, Hitbox{
            .hitbox = glm::vec3(0.5f, 1.5f, 1.0f)
    });
    GCR.add_component(player_laser, Velocity{});
    GCR.add_component(player_laser, Player{});
    GCR.add_component(player_laser, Projectile{
            .damage = 1
    });

    // bombs
    for (int i = 0; i < Entities::P_BOMB_AMT; i++) {
        Entity player_bomb = GCR.create_entity();
        GCR.add_component(player_bomb, State{});
        GCR.add_component(player_bomb, SpriteCache::get_sprite("pbomb"));
        GCR.add_component(player_bomb, Transform{});
        GCR.add_component(player_bomb, Projectile{
                .type = BOMB,
        });
        GCR.add_component(player_bomb, Player{});
        GCR.add_component(player_bomb, Hitbox{
                .hitbox = glm::vec3(3000.0f)
        });
    }

    // bullets
    for (int i = 0; i < Entities::P_BULLET_AMT; i++) {
        Entity player_bullet = GCR.create_entity();
        GCR.add_component(player_bullet, State{
                .active = false // redundant
        });
        GCR.add_component(player_bullet, SpriteCache::get_sprite("pbullet"));

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

}

void Game::init_ecs() {
    // register components
    GCR.register_component<Sprite>();
    GCR.register_component<Particle>();
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

    background_system = GCR.register_system<BackgroundSystem>();
    {
        Signature signature;
        signature.set(GCR.get_component_type<Particle>());
        GCR.set_system_signature<BackgroundSystem>(signature);
    }

    background_system->init();
}