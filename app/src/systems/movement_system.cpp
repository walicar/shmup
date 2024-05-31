#include "movement_system.h"
#include "../ecs/coordinator.h"
#include <iostream>
#include "../components/transform.h"
#include "../components/state.h"
#include "../components/tags/player.h"

extern Coordinator GCR;

void MovementSystem::init() {
    GCR.add_listener(METHOD_LISTENER(Events::Window::INPUT, MovementSystem::input));
}

void MovementSystem::update(float dt) {
    float factor = 10.0f;
    auto &player_rotate = GCR.get_component<Player>(Entities::PLAYER).rotate;

    if (buttons.test(static_cast<std::size_t>(InputButtons::SHIFT))) {
        factor = 3.0f;
        player_rotate = false;
    } else {
        player_rotate = true;
    }

    auto &transform = GCR.get_component<Transform>(Entities::PLAYER);
    if (buttons.test(static_cast<std::size_t>(InputButtons::W)) && dt * factor + transform.pos.y < 9.5f) {
        transform.pos.y += (dt * factor);
    }

    if (buttons.test(static_cast<std::size_t>(InputButtons::S)) && dt * factor + transform.pos.y > -9.5f) {
        transform.pos.y -= (dt * factor);
    }

    if (buttons.test(static_cast<std::size_t>(InputButtons::A)) && dt * factor + transform.pos.x > -9.5f) {
        transform.pos.x -= (dt * factor);
    }

    if (buttons.test(static_cast<std::size_t>(InputButtons::D)) && dt * factor + transform.pos.x < 9.5f) {
        transform.pos.x += (dt * factor);
    }
}


void MovementSystem::input(Event &e) {
    buttons = e.get_param<std::bitset<8>>(Events::Window::Input::INPUT);
}

