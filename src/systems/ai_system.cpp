#include "ai_system.h"
#include "../ecs/coordinator.h"
#include "../components/transform.h"

extern Coordinator GCR;

void AISystem::update(float time) {
    for (auto& entity : entities) {
        auto& pos = GCR.get_component<Transform>(entity).pos;
        pos.x = glm::sin(time) * 2.0f;
    }
}

void AISystem::init() {};

