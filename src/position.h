#ifndef POSITION_H
#define POSITION_H
#include "component.h"
#include <gtc/matrix_transform.hpp>

class Position : public Component {
    glm::vec2 position;
    Position(size_t id, glm::vec2 position) : Component(id), position(position) {}
};

#endif
