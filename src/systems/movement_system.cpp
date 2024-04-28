#include "movement_system.h"
#include "../ecs/coordinator.h"
#include <iostream>
#include "../components/transform.h"

extern Coordinator GCR;

void MovementSystem::init() {
    GCR.add_listener(METHOD_LISTENER(Events::Window::INPUT, MovementSystem::input));
}

void MovementSystem::update(float dt)
{
        float factor = 10.0f;
        for (auto& entity : entities) {

            if (buttons.test(static_cast<std::size_t>(InputButtons::SHIFT))) {
                factor = 3.0f;
                std::cout << " L_SHIFT ";
            }

            auto &transform = GCR.get_component<Transform>(entity);
            std::cout << dt << " - CtrlSys: ";
            if (buttons.test(static_cast<std::size_t>(InputButtons::W))) {
                transform.pos.y += (dt * factor);
                std::cout << " W ";
            }

            if (buttons.test(static_cast<std::size_t>(InputButtons::S))) {
                transform.pos.y -= (dt * factor);
                std::cout << " S ";
            }

            if (buttons.test(static_cast<std::size_t>(InputButtons::A))) {
                transform.pos.x -= (dt * factor);
                std::cout << " A ";
            }

            if (buttons.test(static_cast<std::size_t>(InputButtons::D))) {
                transform.pos.x += (dt * factor);
                std::cout << " D ";
            }

//            // shooting
//            if (buttons.test(static_cast<std::size_t>(InputButtons::J))) {
//                std::cout << " J ";
//            }
//
//            if (buttons.test(static_cast<std::size_t>(InputButtons::K))) { // laser
//                std::cout << " K ";
//            }
//
//            if (buttons.test(static_cast<std::size_t>(InputButtons::L))) {
//                std::cout << " L ";
//            }
            std::cout << "\n";
        }
}



void MovementSystem::input(Event& e) {
    buttons = e.get_param<std::bitset<8>>(Events::Window::Input::INPUT);
}

