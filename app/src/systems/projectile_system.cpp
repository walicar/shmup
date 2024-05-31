#include "projectile_system.h"

#include "../ecs/coordinator.h"
#include <iostream>
#include "../components/transform.h"
#include "../components/velocity.h"
#include "../components/sprite.h"
#include "../components/projectile.h"
#include "../components/state.h"

extern Coordinator GCR;

void ProjectileSystem::init() {
    GCR.add_listener(METHOD_LISTENER(Events::Window::INPUT, ProjectileSystem::input));
    GCR.add_listener(METHOD_LISTENER(Events::Game::STOP, ProjectileSystem::reset_bombs));
}

void ProjectileSystem::update(float time) {
    for (auto &entity: entities) {
        if (buttons.test(static_cast<std::size_t>(InputButtons::J)) && entity >= Entities::P_BULLET &&
            entity < Entities::E_GRUNT) {
            if (bullet_last_shot + bullet_cooldown > time)
                continue;
            auto &bullet_sprite = GCR.get_component<Sprite>(entity);
            auto &bullet_state = GCR.get_component<State>(entity);
            auto &projectile_last_shot = GCR.get_component<Projectile>(entity).last_shot;
            if (!bullet_state.active) {
                glm::vec3 player_location = GCR.get_component<Transform>(Entities::PLAYER).pos;
                glm::vec3 player_scale = GCR.get_component<Sprite>(Entities::PLAYER).scale_factor;
                auto &transform = GCR.get_component<Transform>(entity);
                transform.pos.y = player_location.y + (6.0f * player_scale.y);
                transform.pos.x = player_location.x;
                auto &velocity = GCR.get_component<Velocity>(entity);
                velocity.force = glm::vec3(0.0f, 10.0f, 0.0f);
                bullet_state.active = true;
                bullet_sprite.scale_factor = player_scale;
                bullet_last_shot = time;
                projectile_last_shot = time;
            }

            // projectile lifetime
            if (projectile_last_shot + bullet_cooldown + 2 < time) {
                bullet_state.active = false;
            }
        } else if (entity == Entities::P_LASER) {
            auto &laser_sprite = GCR.get_component<Sprite>(entity);
            auto &laser_state = GCR.get_component<State>(entity);
            if (buttons.test(static_cast<std::size_t>(InputButtons::K))) {
                glm::vec3 player_location = GCR.get_component<Transform>(Entities::PLAYER).pos;
                glm::vec3 player_scale = GCR.get_component<Sprite>(Entities::PLAYER).scale_factor;
                auto &transform = GCR.get_component<Transform>(entity);
                transform.pos.y = player_location.y + (14.0f * player_scale.y);
                transform.pos.x = player_location.x;
                laser_state.active = true;
                laser_sprite.scale_factor = player_scale;
            } else {
                laser_state.active = false;
            }
        }

        if (entity == Entities::P_BOMB) {
            if (bullet_last_shot + bullet_cooldown > time) continue;
            auto &bomb_state = GCR.get_component<State>(entity);
            auto &projectile_last_shot = GCR.get_component<Projectile>(entity).last_shot;
            if (buttons.test(static_cast<std::size_t>(InputButtons::L))) {
                if (bomb_offset > 2) continue; // can't shoot anymore bombs
                Event event(Events::Game::BOMB_USED); // update UI
                GCR.send_event(event);
                glm::vec3 player_location = GCR.get_component<Transform>(Entities::PLAYER).pos;
                glm::vec3 player_scale = GCR.get_component<Sprite>(Entities::PLAYER).scale_factor;
                auto &transform = GCR.get_component<Transform>(entity);
                transform.pos.y = player_location.y + (6.0f * player_scale.y);
                transform.pos.x = player_location.x;
                bomb_state.active = true;
                projectile_last_shot = time;
                bomb_offset += 1;
                bullet_last_shot = time;
            }

            // projectile lifetime
            if (projectile_last_shot + bullet_cooldown + 0.5f < time) {
                bomb_state.active = false;
            }
        }
    }
}

void ProjectileSystem::input(Event &e) {
    buttons = e.get_param<std::bitset<8>>(Events::Window::Input::INPUT);
}

void ProjectileSystem::reset_bombs(Event &e) {
    bomb_offset = 0;
}

