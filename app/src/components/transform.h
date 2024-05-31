#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

struct Transform {
  glm::vec3 pos;
  glm::vec3 origin;
};

#endif