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
    static T& get(size_t id) {
        for (Entry entry : data) {
            if ( id == entry.comp.get_id() && entry.valid) {
                return entry;
            }
        }
        return nullptr;
    };

    static void insert(T comp) {
        data.push_back(comp);
    };

    static void remove(size_t id) {
        size_t it = std::find_if(data.begin(), data.end(), [id](const Entry& entry) {return entry.get_id() == id;});
        if (it != data.end()) {
            data.erase(it);
        }
    }

    static void fixed_update() {
        for (Entry& entry : data) {
            if (entry.valid) {
                entry.comp.fixed_update();
            }
        }
    }

    static void delta_update(float delta) {
        for (Entry& entry : data) {
            if (entry.valid) {
                entry.comp.delta_update(delta);
            }
        }
    }
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