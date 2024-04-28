#ifndef CONTROL_SYSTEM_H
#define CONTROL_SYSTEM_H

#include <bitset>
#include "../ecs/types.h"
#include "../ecs/system.h"
#include "../ecs/event.h"

class ControlSystem : public System
{
    public:
        void init();
        void update(float time);

    private:
        std::bitset<8> buttons;
        void input(Event& e);
};


#endif