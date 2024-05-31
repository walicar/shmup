#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include "../ecs/event.h"
#include "../ecs/system.h"
#include "../ecs/types.h"
#include <bitset>

class MovementSystem : public System {
public:
  void init();

  void update(float time);

private:
  std::bitset<8> buttons;

  void input(Event &e);
};

#endif