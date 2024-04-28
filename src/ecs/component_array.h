#ifndef COMPONENT_QUEUE_H
#define COMPONENT_QUEUE_H

// @TODO: opitmize this into a DoublyLinkedList?

#include "types.h"

/**
 * An interface is needed so that the ComponentManager
 *  can tell a generic ComponentArray that an entity has been destroyed
 *  and that it needs to update its array mappings.
*/

class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void destroyed(Entity entity) = 0;
};

/**
 * The packed array of components (of generic type T), 
 * set to a specified maximum amount, matching the maximum number
 * of entities allowed to exist simultaneously, so that each entity
 * has a unique spot.
*/
template<typename T>
class ComponentArray : public IComponentArray
{
    private:
        
        std::array<T, MAX_ENTITIES> array;

        // Map from an entity ID to an array index.
        std::unordered_map<Entity, size_t> ei;

        // Map from an array index to an entity ID.
        std::unordered_map<size_t, Entity> ie;

        size_t size;
    public:
        void append(Entity entity, T component)
        {
            // Put new entry at end and update the maps
            size_t new_index = size;
            ei[entity] = new_index;
            ie[new_index] = entity;
            array[new_index] = component;
            ++size;
        }

        void remove(Entity entity)
        {
            //assert(ei.find(entity) != ei.end() && "Removing non-existent component.");

            // Copy element at end into deleted element's place to maintain density
            size_t loc = ei[entity]; // index of entity to be removed
            size_t end = size - 1; // index of last element
            array[loc] = array[end];

            // Update map to point to moved spot
            Entity last = ie[end];
            ei[last] = loc;
            ie[loc] = last;

            ei.erase(entity);
            ie.erase(end);

            --size;
        }

        T& get(Entity entity)
        {
            // assert(ei.find(entity) != ei.end() && "Retrieving non-existent component.");

            // Return a reference to the entity's component
            return array[ei[entity]];
        }

        void destroyed(Entity entity) override
        {
            if (ei.find(entity) != ei.end())
            {
                // Remove the entity's component if it existed
                remove(entity);
            }
        }


};
#endif