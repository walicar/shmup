#include "movement_system.h"
#include "../ecs/coordinator.h"
#include <iostream>
#include "../components/transform.h"
#include "../components/state.h"

extern Coordinator GCR;

void MovementSystem::init() {
    GCR.add_listener(METHOD_LISTENER(Events::Window::INPUT, MovementSystem::input));
}

void MovementSystem::update(float dt) {
    float factor = 10.0f;
    auto active = GCR.get_component<State>(Entities::PLAYER).active;

    for (auto &entity: entities) {
        if (!active) continue;

        if (buttons.test(static_cast<std::size_t>(InputButtons::SHIFT))) {
            factor = 3.0f;
        }

        auto &transform = GCR.get_component<Transform>(entity);
        if (buttons.test(static_cast<std::size_t>(InputButtons::W))) {
            transform.pos.y += (dt * factor);
        }

        if (buttons.test(static_cast<std::size_t>(InputButtons::S))) {
            transform.pos.y -= (dt * factor);
        }

        if (buttons.test(static_cast<std::size_t>(InputButtons::A))) {
            transform.pos.x -= (dt * factor);
        }

        if (buttons.test(static_cast<std::size_t>(InputButtons::D))) {
            transform.pos.x += (dt * factor);
        }
    }
}


void MovementSystem::input(Event &e) {
    buttons = e.get_param<std::bitset<8>>(Events::Window::Input::INPUT);
}

