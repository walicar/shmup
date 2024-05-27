#include "background_system.h"
#include "../ecs/coordinator.h"
#include "../components/transform.h"
#include "../components/sprite.h"
#include "../components/velocity.h"
#include "src/components/state.h"

extern Coordinator GCR;

void BackgroundSystem::init() {}

void BackgroundSystem::update(float time) {
    if (last_emit + cooldown < time ) {
        Entity gstar_loc = next_star() + Entities::G_STAR;
        float x_loc = x_distr(gen);
        auto& gstar_movement = GCR.get_component<Transform>(gstar_loc);
        gstar_movement.pos.x = x_loc;
        gstar_movement.pos.y = 9.5f;
        auto& gstar_force = GCR.get_component<Velocity>(gstar_loc).force;
        gstar_force.y = -3.0f;
        auto& gstar_active = GCR.get_component<State>(gstar_loc).active;
        gstar_active = true;
        last_emit = time;
    }
}

Entity BackgroundSystem::next_star() {
    return gstar_offset = (gstar_offset + 1) % Entities::G_STAR_AMT;
}


