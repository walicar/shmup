#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <queue>
#include <array>
#include "entity.h"
#include "component.h"

/**
 * The Entity Manager is in charge of distributing entity IDs 
 * and keeping record of which IDs are in use and which are not.
*/

class EntityManager 
{
    private:
    std::queue<Entity> entities{};
    std::array<Signature, MAX_ENTITIES> signatures{}; 
    uint32_t count; // total living entities

    public:
        EntityManager() 
        {
            for (Entity entity = 0; entity < MAX_ENTITIES; entity++) 
            {
                entities.push(entity);
            }
        }

        Entity create()
        {
            // @TODO: there is no checking if we have too much entities!
            Entity id = entities.front();
            entities.pop();
            count++;
            return id;
        }

        Entity destroy(Entity entity) 
        {
            entities.push(entity);
            signatures[entity].reset();
            count--;
        }

        void sign(Entity entity, Signature signature)
        {
            signatures[entity] = signature;
        }

        Signature get(Entity entity) {
            return signatures[entity];
        }
};

#endif