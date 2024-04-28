#include "collision_system.h"
#include <iostream>
#include "../ecs/coordinator.h"
#include "../components/hitbox.h"
#include "../components/sprite.h"
#include "../components/tags/enemy.h"
#include "../components/tags/projectile.h"
#include "../components/tags/player.h"
#include "../components/transform.h"

extern Coordinator GCR;

void CollisionSystem::init()
{
    player.set(GCR.get_component_type<Player>());
    enemy.set(GCR.get_component_type<Enemy>());
    projectile.set(GCR.get_component_type<Projectile>());
}

void CollisionSystem::update(float dt)
{
    // for every Player or Enemy
    for (auto& nt1 : entities) {
        if ((GCR.get_signature(nt1) & projectile) == projectile)
            continue;
        // check if it overlaps with an opposing projectile
        for (auto& nt2 : entities) {
            if ((GCR.get_signature(nt2) & projectile) != projectile || nt1 == nt2)
                continue;

            auto& is_proj_active = GCR.get_component<Sprite>(nt2).active;
            if (!is_proj_active)
                continue;

            auto& actor_hbx = GCR.get_component<Hitbox>(nt1).hitbox;
            auto& actor_pos = GCR.get_component<Transform>(nt1).pos;
            auto& proj_hbx = GCR.get_component<Hitbox>(nt2).hitbox;
            auto& proj_pos = GCR.get_component<Transform>(nt2).pos;

            if (overlaps(actor_hbx, actor_pos, proj_hbx, proj_pos)) {
                printf("ColSystem: projectile [%d] overlaps actor [%d]\n", nt2, nt1);
            }
        }
    }
}

bool CollisionSystem::overlaps(glm::vec3 hb1, glm::vec3 pos1, glm::vec3 hb2, glm::vec3 pos2) {
    glm::vec3 max1 = pos1 + hb1;
    glm::vec3 max2 = pos2 + hb2;

    return (pos1.x <= max2.x && max1.x >= pos2.x)
        && (pos1.y <= max2.y && max1.y >= pos2.y)
        &&(pos1.z <= max2.z && max1.z >= pos2.z);
}