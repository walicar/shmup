#include "ai_system.h"
#include "../ecs/coordinator.h"
#include "../components/transform.h"
#include "../components/ai.h"
#include "../components/sprite.h"
#include "../components/velocity.h"
#include "../components/projectile.h"
#include "src/components/state.h"
#include "src/components/hitbox.h"

extern Coordinator GCR;

void AISystem::init() {
    projectile.set(GCR.get_component_type<Projectile>());
}

void AISystem::update(float time) {
    for (auto& entity : entities) {
        auto &is_active = GCR.get_component<State>(entity).active;
        if (!is_active) continue;

        auto &health = GCR.get_component<Hitbox>(entity).health;
        
        if (health <= 0) {
            is_active = false;
        }

        auto &origin = GCR.get_component<Transform>(entity).origin;
        auto &pos = GCR.get_component<Transform>(entity).pos;
        pos.x = (glm::sin(time) * 2.0f) + origin.x;

        auto& ai = GCR.get_component<AI>(entity);
        if (ai.last_attacked + ai.attack_cooldown < time) {
            Entity ebullet_loc = next_bullet() + Entities::E_BULLET;
            auto &ebullet_sprite = GCR.get_component<Sprite>(ebullet_loc);
            auto &ebullet_state = GCR.get_component<State>(ebullet_loc);
            auto &ebullet_transform = GCR.get_component<Transform>(ebullet_loc);
            auto& enemy_scale = GCR.get_component<Sprite>(entity).scale_factor;
            ebullet_transform.pos.y = pos.y + (-6.0f * enemy_scale.y);
            ebullet_transform.pos.x = pos.x;
            ebullet_transform.origin = ebullet_transform.pos;

            auto &velocity = GCR.get_component<Velocity>(ebullet_loc);
            velocity.force = glm::vec3(0.0f, -5.0f, 0.0f);
            ebullet_state.active = true;
            ebullet_sprite.scale_factor = enemy_scale;
            ai.last_attacked = time;
        }
    }
}

Entity AISystem::next_bullet() {
    return ebullet_offset = (ebullet_offset + 1) % Entities::E_BULLET_AMT;
}


