#ifndef PROJECTILE_H
#define PROJECTILE_H

enum ProjectileType {
    BULLET,
    LASER,
};

struct Projectile {
    float last_shot = 0;
    ProjectileType type = BULLET;
};

#endif
