#include "physics_system.h"
#include "../ecs/coordinator.h"
#include "../components/velocity.h"
#include "../components/transform.h"
#include "../components/sprite.h"
#include "../components/state.h"

extern Coordinator GCR;


void PhysicsSystem::update(float dt) {
    for (auto &entity: entities) {
        auto is_active = GCR.get_component<State>(entity).active;
        if (!is_active) continue;
        auto &movement = GCR.get_component<Transform>(entity);
        auto &force = GCR.get_component<Velocity>(entity).force;
        movement.pos += (force * dt);
        movement.origin += (force * dt);
    }
}

void PhysicsSystem::init() {
    //
};