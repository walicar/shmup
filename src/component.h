#ifndef COMPONENT_H
#define COMPONENT_H
#include <vector>
#include "entity.h"

class Component {
    protected:
        size_t id;
    public:
        Component(size_t id) : id(id) {}

        void delta_update(float delta) {}

        void fixed_update() {}

        size_t get_id() const {
            return id;
        }
};

// Used to store components of the same type
template<typename T>
class ComponentSet {
public:
    struct Entry {
        bool valid;
        T comp;
    };
private:
    static std::vector<Entry> data;
public:
    static T& get(size_t id);
    static void insert(T comp);
    static void remove(size_t id);
    static void fixed_update();
    static void delta_update(float delta);
};

// Used to retrieve component sets
template<typename T>
class ComponentHandle : virtual public Entity {
    public:
        template<typename... ARGS>
        ComponentHandle(ARGS... args) {
            ComponentSet<T>::insert(T(id,args...));
        }

        ~ComponentHandle() {
            ComponentSet<T>::remove(id);
        }

        T& get() {
            return ComponentSet<T>::get(id);
        }
};

#endif