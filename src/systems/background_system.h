#ifndef BACKGROUND_SYSTEM_H
#define BACKGROUND_SYSTEM_H
#include "../ecs/system.h"
#include "../ecs/types.h"
#include "../ecs/event.h"
#include <random>

// spawn star particles

class BackgroundSystem : public System {
public:
    BackgroundSystem()
    : gen(std::random_device{}()), x_distr(-10.0, 10.0)
    {}; // uniform init? look more into this

    void init();
    void update(float time);

private:
    size_t gstar_offset = Entities::G_STAR_AMT - 1; // circular
    float last_emit = 0.0f;
    float cooldown = 0.45f;

    Entity next_star();

    std::mt19937 gen;

    std::uniform_real_distribution<> x_distr;


};

#endif
