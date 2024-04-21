#include "component.h"

// if the class is templated, make sure to pass in the T
template<typename T>
T& ComponentSet<T>::get(size_t id) {
    for (Entry entry : data) {
        if ( id == entry.comp.get_id() && entry.valid) {
            return entry;
        }
    }
    return nullptr;
};

template<typename T>
void ComponentSet<T>::insert(T comp) {
    data.push_back(comp);
};

template<typename T>
void ComponentSet<T>::remove(size_t id) {
    size_t it = std::find_if(data.begin(), data.end(), [id](const Entry& entry) {return entry.get_id() == id;});
    if (it != data.end()) {
        data.erase(it);
    }
}

template<typename T>
void ComponentSet<T>::fixed_update() {
    for (Entry& entry : data) {
        if (entry.valid) {
            entry.comp.fixed_update();
        }
    }
}

template<typename T>
void ComponentSet<T>::delta_update(float delta) {
    for (Entry& entry : data) {
        if (entry.valid) {
            entry.comp.delta_update(delta);
        }
    }
}



