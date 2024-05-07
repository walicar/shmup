#include "physics_system.h"
#include "../ecs/coordinator.h"
#include "../components/velocity.h"
#include "../components/transform.h"
#include "../components/sprite.h"

extern Coordinator GCR;


void PhysicsSystem::update(float dt) {
    for (auto &entity: entities) {
        auto is_active = GCR.get_component<Sprite>(entity).active;
        if (!is_active) continue;
        // player bullets
        if (entity >= Entities::P_BULLET && entity < Entities::E_GRUNT) {
            auto &movement = GCR.get_component<Transform>(entity);
            auto &force = GCR.get_component<Velocity>(entity).force;
            movement.pos += (force * dt);
        }
        // enemy bullets
        if (entity >= Entities::E_BULLET && entity < Entities::E_TLASER) {
            auto &movement = GCR.get_component<Transform>(entity);
            auto &force = GCR.get_component<Velocity>(entity).force;
            movement.pos += (force * dt);
        }
    }
}

void PhysicsSystem::init() {
    //
};