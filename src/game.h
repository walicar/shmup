#ifndef GAME_H
#define GAME_H

#include "systems/sprite_system.h"
#include "systems/movement_system.h"
#include "systems/projectile_system.h"
#include "systems/physics_system.h"
#include "systems/collision_system.h"
#include "systems/ai_system.h"
#include "systems/animation_system.h"
#include "systems/spawn_system.h"
#include "systems/text_system.h"
#include "src/ecs/coordinator.h"
#include <chrono>
#include <map>

extern Coordinator GCR;

class Game {
public:
    bool in_game = false;
    Game();
    void loop(float dt);
    void reset();
    void start();

private:
    std::shared_ptr<AISystem> ai_system;
    std::shared_ptr<AnimationSystem> animation_system;
    std::shared_ptr<CollisionSystem> collision_system;
    std::shared_ptr<MovementSystem> movement_system;
    std::shared_ptr<PhysicsSystem> physics_system;
    std::shared_ptr<ProjectileSystem> projectile_system;
    std::shared_ptr<SpawnSystem> spawn_system;
    std::shared_ptr<SpriteSystem> sprite_system;
    void input(Event &e);
};

#endif
