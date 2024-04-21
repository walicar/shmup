#ifndef COORDINATOR_H
#define COORDIANTOR_H

#include <memory>
#include "component_manager.h"
#include "entity_manager.h"
#include "system_manager.h"

/**
 * Have the ECS managers talk to each other
*/

class Coordinator 
{
    private:
	    std::unique_ptr<EntityManager> entity_manager;
        std::unique_ptr<ComponentManager> component_manager;
	    std::unique_ptr<SystemManager> system_manager;

    public:
        Coordinator() {
            entity_manager = std::make_unique<EntityManager>();
            component_manager = std::make_unique<ComponentManager>();
            system_manager = std::make_unique<SystemManager>();
        }

        // Entity

        Entity create_entity()
        {
            return entity_manager->create_entity();
        }

        void destroy_entity(Entity entity)
        {
            entity_manager->destroy(entity);
            component_manager->destroyed(entity);
            system_manager->destroyed(entity);
        }

        // Component

        template<typename T>
        void register_component()
        {
            component_manager->register_component<T>();
        }

        template<typename T>
        void add_component(Entity entity, T component)
        {
            component_manager->add_component<T>(entity, component);

            auto signature = entity_manager->get_signature(entity);
            signature.set(component_manager->get_type<T>(), true);
            entity_manager->set_signature(entity, signature);

            system_manager->change_signature(entity, signature);
        }

        template<typename T>
        void remove_component(Entity entity)
        {
            component_manager->remove_component<T>(entity);

            auto signature = entity_manager->get_signature(entity);
            signature.set(component_manager->get_type<T>(), false);
            entity_manager->set_signature(entity, signature);

            system_manager->change_signature(entity, signature);
        }

        template<typename T>
        T& get_component(Entity entity)
        {
            return component_manager->get_component<T>(entity);
        }

        template<typename T>
        ComponentType get_component_type()
        {
            return component_manager->get_type<T>();
        }

        // System

        template<typename T>
        std::shared_ptr<T> register_system()
        {
            return system_manager->register_system<T>();
        }

        template<typename T>
        void set_system_signature(Signature signature)
        {
            system_manager->set_signature<T>(signature);
        }

};


#endif