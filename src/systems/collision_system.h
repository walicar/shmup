#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "../ecs/system.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class CollisionSystem : public System {
public:
    void init();

    void update(float time);

private:
    bool overlaps(glm::vec3 hb1, glm::vec3 pos1, glm::vec3 hb2, glm::vec3 pos2);

    Signature player;
    Signature enemy;
    Signature projectile;

};

#endif