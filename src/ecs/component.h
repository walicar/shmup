#ifndef COMPONENT_H
#define COMPONENT_H
#include <cstdint>
#include <bitset>

using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;

// each entity owns a signature, which represents the components they own.
using Signature = std::bitset<MAX_COMPONENTS>; 

#endif