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

enum class InputButtons
{
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


// C++11 compile-time hash of literal strings
constexpr std::uint32_t fnv1a_32(char const* s, std::size_t count)
{
    return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u; // NOLINT (hicpp-signed-bitwise)
}

constexpr std::uint32_t operator "" _hash(char const* s, std::size_t count)
{
    return fnv1a_32(s, count);
}

namespace Events::Window {
    const EventId QUIT = "Events::Window::QUIT"_hash;
    const EventId RESIZED = "Events::Window::RESIZED"_hash;
    const EventId INPUT = "Events::Window::INPUT"_hash;
}

namespace Events::Window::Input {
    const ParamId INPUT = "Events::Window::Input::INPUT"_hash;
}

namespace Events::Window::Resized {
    const ParamId WIDTH = "Events::Window::Resized::WIDTH"_hash;
    const ParamId HEIGHT = "Events::Window::Resized::HEIGHT"_hash;
}


#endif