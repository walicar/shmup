#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include "../ecs/event.h"
#include "../ecs/system.h"
#include "../ecs/types.h"
#include <bitset>
#include <vector>

class AnimationSystem : public System {

public:
  void init();

  void update(float time);
};

#endif
