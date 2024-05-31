#ifndef SPRITE_SYSTEM_H
#define SPRITE_SYSTEM_H

#include "../ecs/system.h"

class SpriteSystem : public System {
private:
    std::vector<Entity> trash;
public:
    void update(float time);
};

#endif