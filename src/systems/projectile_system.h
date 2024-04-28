#ifndef PROJECTILE_SYSTEM_H
#define PROJECTILE_SYSTEM_H

#include <bitset>
#include <vector>
#include "../ecs/system.h"
#include "../ecs/types.h"
#include "../ecs/event.h"

/**
 * All entities inside this system are inactive/active projectiles
*/

class ProjectileSystem : public System {

public:
    void init();
    void update(float time);

private:
    std::bitset<8> buttons;
    void input(Event& e);

};


#endif
