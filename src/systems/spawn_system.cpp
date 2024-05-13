#include "spawn_system.h"
#include "../ecs/coordinator.h"
#include "../components/sprite.h"
#include "../components/transform.h"
#include "src/components/state.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

extern Coordinator GCR;

// try to make this an event system;

void SpawnSystem::init() {}


void SpawnSystem::update() {
    if (is_done(checkpoint)) {
        checkpoint += 1;
        spawn_checkpoint(checkpoint);
    }
}

bool SpawnSystem::is_done(int checkpoint) {
    for (int i = 0; i < Entities::E_AMT; i++) {
        int entity = i + (Entities::E_AMT * checkpoint);
        bool active = GCR.get_component<State>(entity + Entities::E_GRUNT).active;
        if (active) return false;
    }
    return true;
}

bool SpawnSystem::spawn_checkpoint(int checkpoint) {
    if (checkpoint < 5) {
        for (int i = 0; i < Entities::E_AMT; i++) {
            int entity = i + (Entities::E_AMT * checkpoint);
            auto &active = GCR.get_component<State>(entity + Entities::E_GRUNT).active = true;
        }
    } else if (checkpoint == 5) {
        GCR.get_component<State>(Entities::BOSS).active = true;
    }


}
