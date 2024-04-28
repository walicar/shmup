#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "../ecs/system.h"

class PhysicsSystem : public System {
public:
    void update(float dt);
    void init();
};


#endif
