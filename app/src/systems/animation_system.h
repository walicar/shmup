#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include <bitset>
#include <vector>
#include "../ecs/system.h"
#include "../ecs/types.h"
#include "../ecs/event.h"

class AnimationSystem : public System {

public:
    void init();

    void update(float time);
};


#endif
