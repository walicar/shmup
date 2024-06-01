#include "collision_system.h"
#include "../components/hitbox.h"
#include "../components/projectile.h"
#include "../components/sprite.h"
#include "../components/state.h"
#include "../components/transform.h"
#include "../ecs/coordinator.h"
#include <iostream>

extern Coordinator GCR;

void CollisionSystem::init() {
  projectile.set(GCR.get_component_type<Projectile>());
}

void CollisionSystem::update(float dt) {
  // for every Player or Enemy
  for (auto &nt1 : entities) {
    auto &actor_active = GCR.get_component<State>(nt1).active;
    if (!actor_active)
      continue;
    if ((GCR.get_signature(nt1) & projectile) == projectile)
      continue;
    // check if it overlaps with an opposing projectile
    for (auto &nt2 : entities) {
      if ((GCR.get_signature(nt2) & projectile) != projectile || nt1 == nt2)
        continue;

      auto &is_proj_active = GCR.get_component<State>(nt2).active;
      if (!is_proj_active)
        continue;

      auto &actor_hbx = GCR.get_component<Hitbox>(nt1).hitbox;
      auto &actor_pos = GCR.get_component<Transform>(nt1).pos;
      auto &actor_hp = GCR.get_component<Hitbox>(nt1).health;
      auto &proj_hbx = GCR.get_component<Hitbox>(nt2).hitbox;
      auto &proj_pos = GCR.get_component<Transform>(nt2).pos;
      auto &proj_type = GCR.get_component<Projectile>(nt2).type;
      auto &proj_dmg = GCR.get_component<Projectile>(nt2).damage;
      auto &proj_active = GCR.get_component<State>(nt2).active;

      if (overlaps(actor_hbx, actor_pos, proj_hbx,
                   proj_pos)) { // calculate damage
        int damage = proj_dmg;
        if (proj_type == BULLET) {
          proj_active = false;
        }

        actor_hp -= damage;
        if (actor_hp <= 0) {
          actor_active = false;
          if (nt1 == Entities::BOSS) {
            Event event(Events::Game::STOP);
            GCR.send_event(event);
          }
        }

        // printf("ColSystem: projectile [%d] overlaps actor [%d], HP [%d]\n",
        // nt2, nt1, actor_hp);
      }
    }
  }
}

bool CollisionSystem::overlaps(glm::vec3 hb1, glm::vec3 pos1, glm::vec3 hb2,
                               glm::vec3 pos2) {
  glm::vec3 max1 = pos1 + hb1;
  glm::vec3 max2 = pos2 + hb2;

  return (pos1.x <= max2.x && max1.x >= pos2.x) &&
         (pos1.y <= max2.y && max1.y >= pos2.y) &&
         (pos1.z <= max2.z && max1.z >= pos2.z);
}
