#ifndef SYSTEM_H
#define SYSTEM_H

#include "types.h"
#include <set>

/**
 * Provide functionality to iterate upon a list of entities with a variable
 * signature
 */

class System {
public:
  std::set<Entity> entities;
};

#endif