#include "physics_system.h"
#include "../ecs/coordinator.h"
#include "../components/velocity.h"
#include "../components/transform.h"

extern Coordinator GCR;


void PhysicsSystem::update(float dt) {
    for (auto &entity: entities) {
        if (entity >= 2 && entity <= 11) {
            auto &movement = GCR.get_component<Transform>(entity);
            auto &force = GCR.get_component<Velocity>(entity).force;
            movement.pos += (force * dt);
        }
    }
}

void PhysicsSystem::init() {
    //
};