#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <queue>
#include <array>
#include "types.h"

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
        EntityManager() : count(0)
       {
            for (Entity entity = 0; entity < MAX_ENTITIES; entity++)
            {
                entities.push(entity);
            }
        }

        Entity create_entity()
        {
            // @TODO: there is no checking if we have too much entities!
            Entity id = entities.front();
            entities.pop();
            count++;
            return id;
        }

        void destroy(Entity entity)
        {
            entities.push(entity);
            signatures[entity].reset();
            count--;
        }

        void set_signature(Entity entity, Signature signature)
        {
            signatures[entity] = signature;
        }

        Signature get_signature(Entity entity) {
            return signatures[entity];
        }
};

#endif