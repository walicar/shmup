#ifndef SPAWN_SYSTEM_H
#define SPAWN_SYSTEM_H

#include "../ecs/system.h"
#include "src/ecs/event.h"

class SpawnSystem : public System {
public:
    void update();

    void init();

private:
    int checkpoint = -1;

    bool static is_done(int checkpoint);

    void static spawn_checkpoint(int checkpoint);

    void start_game(Event &e);

    void stop_game(Event &e);

};


#endif

