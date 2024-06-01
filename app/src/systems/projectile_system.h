#ifndef PROJECTILE_SYSTEM_H
#define PROJECTILE_SYSTEM_H

#include "../ecs/event.h"
#include "../ecs/system.h"
#include "../ecs/types.h"
#include <bitset>
#include <vector>

/**
 * All entities inside this system are inactive/active projectiles
 */

class ProjectileSystem : public System {

public:
  void init();

  void update(float time);

private:
  float bullet_cooldown = 0.2f;
  float bullet_last_shot = 0.0f;
  int bomb_offset = 0;
  std::bitset<8> buttons;

  void erase_enemy_projectiles();

  void input(Event &e);

  void reset_bombs(Event &e);
};

#endif
