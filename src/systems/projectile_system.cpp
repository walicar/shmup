#include "projectile_system.h"

#include "../ecs/coordinator.h"
#include <iostream>
#include "../components/transform.h"
#include "../components/sprite.h"

extern Coordinator GCR;

void ProjectileSystem::init() {
    GCR.add_listener(METHOD_LISTENER(Events::Window::INPUT, ProjectileSystem::input));
}

void ProjectileSystem::update(float dt)
{
    for (auto& entity : entities) {

        if (buttons.test(static_cast<std::size_t>(InputButtons::J))) {
            // spawn projectiles here
        } else if (entity == 1) { // @FIXME: this can be better!!!
            auto& laser_sprite = GCR.get_component<Sprite>(entity);
            if (buttons.test(static_cast<std::size_t>(InputButtons::K))) {
                // we could just bake this in later
                // implement a coordinate system
                glm::vec3 player_location = GCR.get_component<Transform>(0).pos;
                glm::vec3 player_scale = GCR.get_component<Sprite>(0).scale_factor;
                auto& transform = GCR.get_component<Transform>(entity);
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

        std::cout << "\n";
    }
}

void ProjectileSystem::input(Event& e) {
    buttons = e.get_param<std::bitset<8>>(Events::Window::Input::INPUT);
}

