#ifndef ENTITY_H
#define ENTITY_H

#include <atomic>
class Entity {
    protected:
        static std::atomic<size_t> id_counter;
        size_t  id;
    public:
        Entity() {
            id = id_counter.fetch_add(1);
        }
};

#endif