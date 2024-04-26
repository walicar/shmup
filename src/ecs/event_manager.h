#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "event.h"
#include "types.h"
#include <functional>
#include <list>
#include <unordered_map>

class EventManager
{
    private:
        std::unordered_map<EventId, std::list<std::function<void(Event&)>>> listeners;

    public:
        void AddListener(EventId eventId, std::function<void(Event&)> const& listener)
        {
            listeners[eventId].push_back(listener);
        }

        void send_event(Event& event)
        {
            uint32_t type = event.get_type();

            for (auto const& listener : listeners[type])
            {
                listener(event);
            }
        }

        void send_event(EventId eventId)
        {
            Event event(eventId);

            for (auto const& listener : listeners[eventId])
            {
                listener(event);
            }
        }

};

#endif