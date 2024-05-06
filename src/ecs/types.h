#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <bitset>

using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 5000;

using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;

// each entity owns a signature, which represents the components they own.
using Signature = std::bitset<MAX_COMPONENTS>;

enum class InputButtons {
    W,
    A,
    S,
    D,
    J,
    K,
    L,
    SHIFT
};

using EventId = std::uint32_t;
using ParamId = std::uint32_t;

namespace Entities {
    // a UUID for every important entity
    // may represent a start of the range
    const Entity PLAYER = 0;
    const Entity P_LASER = 1;
    const Entity P_BOMB = 2;
    const Entity P_BOMB_AMT = 3;
    const Entity P_BULLET = 5;
    const Entity P_BULLET_AMT = 50;
    // Enemies
    const Entity E_AMT = 25;
    const Entity E_GRUNT = 50;
    const Entity E_SNIPE = 75;
    const Entity E_STAR = 100;
    const Entity E_HOSE = 125;
    const Entity BOSS = 150;
    // Enemy Projectiles
    const Entity E_BULLET = 151;
    const Entity E_BULLET_AMT = 1000;
    const Entity E_TLASER = 1151;
    const Entity E_TLASER_AMT = 100;
}



namespace Events::Window {
    const EventId QUIT = 8080;
    const EventId RESIZED = 8081;
    const EventId INPUT = 8082;
}

namespace Events::Window::Input {
    const ParamId INPUT = 8083;
}

namespace Events::Window::Resized {
    const ParamId WIDTH = 8084;
    const ParamId HEIGHT = 8085;
}

// function binding, find a way to simplify this?
// - create a new function object from an existing function
#define METHOD_LISTENER(EventType, Listener) EventType, std::bind(&Listener, this, std::placeholders::_1)
#define FUNCTION_LISTENER(EventType, Listener) EventType, std::bind(&Listener, std::placeholders::_1)

#endif