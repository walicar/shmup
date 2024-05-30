#ifndef SPRITE_SYSTEM_H
#define SPRITE_SYSTEM_H

#include "../ecs/system.h"

class SpriteSystem : public System {
public:
    void update(float time);
};

#endif