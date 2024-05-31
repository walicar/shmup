#include "animation_system.h"
#include "../components/sprite.h"
#include "../components/state.h"
#include "../ecs/coordinator.h"

extern Coordinator GCR;

void AnimationSystem::update(float time) {
  for (auto &entity : entities) {

    auto &state = GCR.get_component<State>(entity);
    auto &sprite = GCR.get_component<Sprite>(entity);
    if (!state.active)
      continue;
    if (entity == Entities::P_BOMB) {
      if (state.active && sprite.transparency > 0.0f) {
        sprite.transparency -= 0.01f;
      } else {
        sprite.transparency = 1.0f;
      }
    }
  }
}

void AnimationSystem::init() {}