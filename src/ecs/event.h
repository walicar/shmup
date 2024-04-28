#ifndef EVENT_H
#define EVENT_H

#include "types.h"
#include <unordered_map>
#include <any>

class Event {
private:
    EventId type{};
    std::unordered_map<EventId, std::any> data{};

public:
    Event() = delete;

    explicit Event(EventId type) : type(type) {};

    template<typename T>
    void set_param(EventId id, T value) {
        data[id] = value;
    }

    template<typename T>
    T get_param(EventId id) {
        return std::any_cast<T>(data[id]);
    }

    EventId get_type() const {
        return type;
    }

};

#endif