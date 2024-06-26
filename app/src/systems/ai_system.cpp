#include "ai_system.h"
#include "../components/ai.h"
#include "../components/projectile.h"
#include "../components/sprite.h"
#include "../components/state.h"
#include "../components/tags/enemy.h"
#include "../components/transform.h"
#include "../components/velocity.h"
#include "../ecs/coordinator.h"
#include <random>

extern Coordinator GCR;

void AISystem::init() {}

bool coin_flip() {
  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_int_distribution<> distr(0, 1);

  return static_cast<bool>(distr(eng));
}

void AISystem::update(float time) {
  for (auto &entity : entities) {
    auto &is_active = GCR.get_component<State>(entity).active;
    if (!is_active)
      continue;

    auto &origin = GCR.get_component<Transform>(entity).origin;
    auto &pos = GCR.get_component<Transform>(entity).pos;
    auto &type = GCR.get_component<Enemy>(entity).type;
    if (entity < Entities::E_BULLET) {
      auto scale = 2.0f;
      if (entity == Entities::BOSS) {
        scale *= 4.75f;
      }
      pos.x = (glm::sin(time * 0.95f) * scale) + origin.x;
    }

    auto &ai = GCR.get_component<AI>(entity);
    if (ai.last_attacked + ai.attack_cooldown < time &&
        entity < Entities::E_BULLET) {
      Entity ebullet_loc = next_bullet() + Entities::E_BULLET;
      // printf("[%d] is shooting bullet [%d]\n", entity, ebullet_loc);
      auto &ebullet_sprite = GCR.get_component<Sprite>(ebullet_loc);
      auto &ebullet_state = GCR.get_component<State>(ebullet_loc);
      auto &ebullet_proj = GCR.get_component<Projectile>(ebullet_loc);
      auto &ebullet_transform = GCR.get_component<Transform>(ebullet_loc);
      auto &enemy_scale = GCR.get_component<Sprite>(entity).scale_factor;
      ebullet_transform.pos.y = pos.y + (-6.0f * enemy_scale.y);
      ebullet_transform.pos.x = pos.x;
      ebullet_transform.origin = ebullet_transform.pos;

      auto &velocity = GCR.get_component<Velocity>(ebullet_loc);

      if (type == GRUNT) {
        ebullet_proj.damage = 10;
        velocity.force = glm::vec3(0.0f, -5.0f, 0.0f);
      } else if (type == SNIPE) {
        ebullet_proj.damage = 100;
        auto &player_pos = GCR.get_component<Transform>(Entities::PLAYER).pos;
        glm::vec3 direction =
            glm::normalize(player_pos - ebullet_transform.origin);
        float speed = 5.0f;
        velocity.force = direction * speed;
      } else if (type == HOSE) {
        ebullet_proj.damage = 2;
        auto &player_pos = GCR.get_component<Transform>(Entities::PLAYER).pos;
        glm::vec3 offset(1.0f, 0.0f, 0.0f);
        glm::vec3 modified_player_pos = player_pos + offset;
        glm::vec3 direction =
            glm::normalize(modified_player_pos - ebullet_transform.origin);
        float speed = 1.0f;
        velocity.force = direction * speed;
      } else if (type == STAR) {
        ebullet_proj.damage = 15;
        if (coin_flip()) {
          velocity.force = glm::vec3(3.0f, -10.0f, 0.0f);
        } else {
          velocity.force = glm::vec3(-3.0f, -10.0f, 0.0f);
        }
      } else if (type == BOSS) {
        if (coin_flip()) {
          ebullet_proj.damage = 20;
          auto &player_pos = GCR.get_component<Transform>(Entities::PLAYER).pos;
          glm::vec3 direction =
              glm::normalize(player_pos - ebullet_transform.origin);
          float speed = 7.0f;
          velocity.force = direction * speed;
        } else {
          if (coin_flip()) {
            ebullet_proj.damage = 50;
            velocity.force = glm::vec3(0.0f, -0.5f, 0.0f);
          } else {
            ebullet_proj.damage = 10;
            velocity.force = glm::vec3(0.0f, -10.0f, 0.0f);
          }
        }
      }

      ebullet_state.active = true;
      ebullet_sprite.scale_factor = enemy_scale;
      ai.last_attacked = time;
    }
  }
}

Entity AISystem::next_bullet() {
  return ebullet_offset = (ebullet_offset + 1) % Entities::E_BULLET_AMT;
}
