#ifndef AI_H
#define AI_H

struct AI {
    float attack_cooldown = 2.0f; // default cooldown
    float burst_cooldown = 0.0f;
    float last_attacked = 0.0f;
};

#endif
