#ifndef HITBOX_H
#define HITBOX_H

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

struct Hitbox {
    int health = 100;
    glm::vec3 hitbox;
};

#endif