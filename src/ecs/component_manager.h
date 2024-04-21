#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H
#include <unordered_map>
#include <memory>
#include "component.h"
#include "component_array.h"

/**
 * Component Manager is in charge of handling all of the component arrays
 * when a component needs to be added or removed
*/

class ComponentManager
{
    private:
        std::unordered_map<const char*, ComponentType> types;
        std::unordered_map<const char*, std::shared_ptr<IComponentArray>> arrays;
        
        ComponentType next_type;

        template<typename T>
        std::shared_ptr<ComponentArray<T>> get_array() {
            const char* type = typeid(T).name();

            // @TODO: handle case where you can't find array

            return std::static_pointer_cast<ComponentArray<T>>(arrays[type]);

        }
    public:
        template<typename T>
        void create() {
            const char* type = typeid(T).name();

            // @TODO: handle case when you register a comp more than once

            types.insert({type, next_type});
            arrays.insert({type, std::make_shared<ComponentArray<T>>()});
            next_type++;
        }

        template<typename T>
        ComponentType get_type()
        {
            const char* type = typeID(T).name();
            return types[type]
        }

        	template<typename T>

        void add_component(Entity entity, T component)
        {
            // Add a component to the array for an entity
            get_component<T>()->append(entity, component);
        }

        template<typename T>
        void remove_component(Entity entity)
        {
            // Remove a component from the array for an entity
            get_component<T>()->remove(entity);
        }

        template<typename T>
        T& get_component(Entity entity)
        {
            // Get a reference to a component from the array for an entity
            return get_array<T>()->get(entity);
        }

        void destroyed(Entity entity)
        {
            // Notify each component array that an entity has been destroyed
            // If it has a component for that entity, it will remove it
            for (auto const& pair : arrays)
            {
                auto const& component = pair.second;
                component->destroyed(entity);
            }
        }



};

#endif