#pragma once

#include "Event.h"

namespace Engine
{
    /**
     * @brief An event that represents a key press.
     *
     * This event is dispatched when a key is pressed.
     */
    class KeyPressedEvent : public Event
    {
    public:
        /**
         * @brief Creates a new KeyPressedEvent.
         *
         * This constructor creates a new KeyPressedEvent with the specified key
         * code and repeat flag.
         *
         * @param keyCode The key code of the key that was pressed.
         * @param repeat True if the key is held, false otherwise.
         */
        KeyPressedEvent(int keyCode, bool repeat)
            : keyCode(keyCode), repeat(repeat)
        {
        }

        /**
         * @brief Gets the name of the event.
         *
         * @return The name of the event.
         */
        inline const char* getName() const override
        {
            return "KeyPressedEvent";
        }

        /**
         * @brief Returns the event as a string.
         *
         * @return The event as a string.
         */
        inline std::string toString() const override
        {
            return "KeyPressedEvent: " + std::to_string(keyCode);
        }

        /**
         * @brief Gets the key code of the event.
         *
         * @return The key code of the event.
         */
        inline int getKeyCode() const { return keyCode; }

        /**
         * @brief Checks if the key is held.
         *
         * @return True if the key is held, false otherwise.
         */
        inline bool isKeyHeld() const { return repeat; }

    private:
        // The key code of the key that was pressed
        int keyCode;
        // True if the key is held, false otherwise
        bool repeat;
    };

    /**
     * @brief An event that represents a key release.
     *
     * This event is dispatched when a key is released.
     */
    class KeyReleasedEvent : public Event
    {
    public:
        /**
         * @brief Creates a new KeyReleasedEvent.
         *
         * This constructor creates a new KeyReleasedEvent with the specified
         * key code.
         *
         * @param keyCode The key code of the key that was released.
         */
        KeyReleasedEvent(int keyCode) : keyCode(keyCode) {}

        inline const char* getName() const override
        {
            return "KeyReleasedEvent";
        }

        inline std::string toString() const override
        {
            return "KeyReleasedEvent: " + std::to_string(keyCode);
        }

        /**
         * @brief Gets the key code of the event.
         *
         * @return The key code of the event.
         */
        inline int getKeyCode() const { return keyCode; }

    private:
        // The key code of the key that was released
        int keyCode;
    };
}  // namespace Engine
