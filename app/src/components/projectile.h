#ifndef PROJECTILE_H
#define PROJECTILE_H

enum ProjectileType {
  BULLET,
  LASER,
  BOMB,
};

struct Projectile {
  float last_shot = 0;
  ProjectileType type = BULLET;
  int damage = 0;
  float kill_at = -1.0f;
};

#endif
