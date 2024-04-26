#ifndef EVENT_H
#define EVENT_H

#include "types.h"
#include <unordered_map>
#include <any>


class Event
{
    private:
        EventId type {};
        std::unordered_map<EventId, std::any> data {};

    public:
        Event() = delete;

        explicit Event(EventId type) : type(type) {};

        template<typename T>
        void SetParam(EventId id, T value)
        {
            data[id] = value;
        }

        template<typename T>
        T GetParam(EventId id)
        {
            return std::any_cast<T>(data[id]);
        }

        EventId GetType() const
        {
            return type;
        }

};

#endif