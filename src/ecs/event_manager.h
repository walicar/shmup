#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "event.h"
#include "types.h"
#include <functional>
#include <list>
#include <unordered_map>

/**
 * EventManager is in charge of running callbacks stored in `listeners`
 *  when the appropriate event shows up
 */

class EventManager
{
    private:
        std::unordered_map<EventId, std::list<std::function<void(Event&)>>> listeners;

    public:
        void add_listener(EventId eventId, std::function<void(Event&)> const& listener)
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