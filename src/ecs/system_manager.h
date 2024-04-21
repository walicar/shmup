#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H
#include <set>
#include <unordered_map>
#include "entity.h"
#include "component.h"

/**
 * Provide functionality to iterate upon a list of entities with a variable signature
*/

class System
{ 
    public:
	    std::set<Entity> entities;
};

/**
 * Maintain a record of registereed systems and signatures.
*/
class SystemManager
{
    private:
        std::unordered_map<const char*, Signature> signatures;
        std::unordered_map<const char*, std::shared_ptr<System>> systems;

    public:
        template<typename T>
        std::shared_ptr<T> register_system()
        {
            const char* type_name = typeid(T).name();

            // @TODO: Handle error when tryign to register a system more than once

            auto system = std::make_shared<T>();
            systems.insert({type_name, system});
            return system;
        }

        template<typename T>
        void set_signature(Signature signature)
        {
            const char* type_name = typeid(T).name();

            // @TODO: Handle error when we can't find system

            signatures.insert({type_name, signature});

        }

        void destroyed(Entity entity) 
        {
            // Erase a destroyed entity from all system lists
		    // mEntities is a set so no check needed
            for (auto const& pair : systems)
            {
                auto const& system = pair.second;
                system->entities.erase(entity);
            }
        }

        void change_signature(Entity entity, Signature entity_signature) 
        {
            // Notify each system that the entity's signature has changed
            for (auto const& pair : systems) 
            {
                auto const& type = pair.first;
                auto const& system = pair.second;
                auto const& system_signature = signatures[type];

                // Entity signature matches system signature - insert into set
                if ((entity_signature & system_signature) == system_signature)
                {
                    system->entities.insert(entity);
                }
                // Entity signature does not match system signature - erase from set
                else
                {
                    system->entities.erase(entity);
                }

            }
        }
};


#endif