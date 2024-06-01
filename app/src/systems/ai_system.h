#ifndef AI_SYSTEM_H
#define AI_SYSTEM_H

#include "../ecs/system.h"

/**
 * handles logic for enemy to shoot at you
 */

class AISystem : public System {
private:
  size_t ebullet_offset = Entities::E_BULLET_AMT - 1; // circular

public:
  void update(float time);

  Entity next_bullet();

  void init();
};

#endif
