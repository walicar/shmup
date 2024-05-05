#include "ai_system.h"
#include "../ecs/coordinator.h"
#include "../components/transform.h"
#include "../components/ai.h"
#include "../components/sprite.h"
#include "../components/velocity.h"

extern Coordinator GCR;


void AISystem::init() {
    ebullet_end = 24;
    ebullet_pos = 14; // this is hardcoded, I will fix this later
};

void AISystem::update(float time) {
    for (auto& entity : entities) {
        auto &origin = GCR.get_component<Transform>(entity).origin;
        auto &pos = GCR.get_component<Transform>(entity).pos;
        pos.x = (glm::sin(time) * 2.0f) + origin.x;

        auto& ai = GCR.get_component<AI>(entity);
        if (ai.last_attacked + ai.attack_cooldown < time && ebullet_pos < ebullet_end) {
            auto &ebullet_sprite = GCR.get_component<Sprite>(ebullet_pos);
            auto &ebullet_transform = GCR.get_component<Transform>(ebullet_pos);
            glm::vec3 enemy_location = GCR.get_component<Transform>(entity).pos;
            glm::vec3 enemy_scale = GCR.get_component<Sprite>(entity).scale_factor;
            ebullet_transform.pos.y = enemy_location.y + (-6.0f * enemy_scale.y);
            ebullet_transform.pos.x = enemy_location.x;
            auto &velocity = GCR.get_component<Velocity>(ebullet_pos);
            velocity.force = glm::vec3(0.0f, -5.0f, 0.0f);
            ebullet_sprite.active = true;
            ebullet_sprite.scale_factor = enemy_scale;
            ai.last_attacked = time;
        }
    }
}


