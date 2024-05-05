#include "projectile_system.h"

#include "../ecs/coordinator.h"
#include <iostream>
#include "../components/transform.h"
#include "../components/velocity.h"
#include "../components/sprite.h"
#include "../components/projectile.h"

extern Coordinator GCR;

void ProjectileSystem::init() {
    GCR.add_listener(METHOD_LISTENER(Events::Window::INPUT, ProjectileSystem::input));
}

// should use signatures, like collision system to detect which entity we are iterating on...

void ProjectileSystem::update(float time) {
    for (auto &entity: entities) {
        if (buttons.test(static_cast<std::size_t>(InputButtons::J)) && entity >= 2 && entity <= 11) {
            if (bullet_last_shot + bullet_cooldown > time)
                continue;
            auto &bullet_sprite = GCR.get_component<Sprite>(entity);
            auto &projectile_last_shot = GCR.get_component<Projectile>(entity).last_shot;
            if (!bullet_sprite.active) {
                glm::vec3 player_location = GCR.get_component<Transform>(0).pos;
                glm::vec3 player_scale = GCR.get_component<Sprite>(0).scale_factor;
                auto &transform = GCR.get_component<Transform>(entity);
                transform.pos.y = player_location.y + (6.0f * player_scale.y);
                transform.pos.x = player_location.x;
                auto &velocity = GCR.get_component<Velocity>(entity);
                velocity.force = glm::vec3(0.0f, 6.0f, 0.0f);
                bullet_sprite.active = true;
                bullet_sprite.scale_factor = player_scale;
                bullet_last_shot = time;
                projectile_last_shot = time;
            }
            if (projectile_last_shot + bullet_cooldown + 2 < time) { // fix this
                bullet_sprite.active = false;
            }
        } else if (entity == 1) { // @FIXME: this can be better!!!
            auto &laser_sprite = GCR.get_component<Sprite>(entity);
            if (buttons.test(static_cast<std::size_t>(InputButtons::K))) {
                // we could just bake this in later
                // implement a coordinate system
                glm::vec3 player_location = GCR.get_component<Transform>(0).pos;
                glm::vec3 player_scale = GCR.get_component<Sprite>(0).scale_factor;
                auto &transform = GCR.get_component<Transform>(entity);
                transform.pos.y = player_location.y + (6.0f * player_scale.y);
                transform.pos.x = player_location.x;
                laser_sprite.active = true;
                laser_sprite.scale_factor = player_scale;
            } else {
                laser_sprite.active = false;
            }
        }

        if (buttons.test(static_cast<std::size_t>(InputButtons::L))) {
            // bomb here
        }
    }
}

void ProjectileSystem::input(Event &e) {
    buttons = e.get_param<std::bitset<8>>(Events::Window::Input::INPUT);
}

