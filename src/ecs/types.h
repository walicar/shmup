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



#endif