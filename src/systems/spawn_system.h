#ifndef SPAWN_SYSTEM_H
#define SPAWN_SYSTEM_H

#include "../ecs/system.h"

class SpawnSystem : public System {
public:
    void update();

    void init();

private:
    int checkpoint = 0;
    int retry = 0;

    bool static is_done(int checkpoint);

    bool static spawn_checkpoint(int checkpoint);

};


#endif

