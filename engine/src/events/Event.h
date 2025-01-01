#pragma once

#include <functional>
#include <string>
#include <typeindex>
#include <unordered_map>

namespace Engine
{
    /**
     * @brief The base class for all events.
     *
     * This class is the base class for all events. It provides a simple
     * interface for getting the name of the event and converting the event to a
     * string.
     */
    class Event
    {
    public:
        /**
         * @brief Gets the name of the event.
         *
         * This method returns the name of the event.
         *
         * @return The name of the event.
         */
        virtual const char* getName() const = 0;

        /**
         * @brief Converts the event to a string.
         *
         * This method converts the event to a string.
         *
         * @return The event as a string.
         */
        virtual std::string toString() const { return getName(); }
    };

    /**
     * @brief A class that dispatches events to event handlers.
     *
     * This class provides a simple interface for dispatching events to event
     * handlers.
     */
    class EventDispatcher
    {
    public:
        // The event handler function type
        using EventHandler = std::function<void(Event&)>;

        /**
         * @brief Adds an event handler for the specified type.
         *
         * This method adds an event handler for the specified event type.
         *
         * @tparam T The type of the event.
         * @param handler The event handler to add.
         */
        template <typename T>
        void addHandler(const std::function<void(T&)>& handler)
        {
            // T must be a subclass of Event
            // We use static_assert to check this at compile time
            static_assert(std::is_base_of<Event, T>::value,
                          "T must be a subclass of Event");

            // Get the type of the event
            std::type_index type = typeid(T);
            // Add the handler to the map
            handlers[type] = [handler](Event& event)
            { handler(static_cast<T&>(event)); };
        }

        /**
         * @brief Dispatches the specified event.
         *
         * This method dispatches the specified event to the appropriate event
         * handler.
         *
         * @param event The event to dispatch.
         */
        void dispatch(Event& event) const;

    private:
        // Map of event types to event handlers
        std::unordered_map<std::type_index, EventHandler> handlers;
    };
}  // namespace Engine