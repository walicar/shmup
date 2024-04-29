#ifndef AI_SYSTEM_H
#define AI_SYSTEM_H

#include "../ecs/system.h"

/**
 * handles logic for enemy to shoot at you
 */

class AISystem : public System {
public:
    void update(float time);
    void init();
};


#endif
