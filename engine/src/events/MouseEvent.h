#pragma once

#include "Event.h"

namespace Engine
{
    /**
     * @brief An event that represents a mouse moved event.
     *
     * This event is dispatched when the mouse is moved.
     */
    class MouseMovedEvent : public Event
    {
    public:
        /**
         * @brief Creates a new MouseMovedEvent.
         *
         * This constructor creates a new MouseMovedEvent with the specified x
         * and y coordinates.
         *
         * @param x The x coordinate of the mouse.
         * @param y The y coordinate of the mouse.
         */
        MouseMovedEvent(float x, float y) : x(x), y(y) {}

        inline const char* getName() const override
        {
            return "MouseMovedEvent";
        }

        std::string toString() const override
        {
            return "MouseMovedEvent: " + std::to_string(x) + ", " +
                   std::to_string(y);
        }

        /**
         * @brief Gets the x coordinate of the mouse.
         *
         * @return The x coordinate of the mouse.
         */
        inline float getX() const { return x; }

        /**
         * @brief Gets the y coordinate of the mouse.
         *
         * @return The y coordinate of the mouse.
         */
        inline float getY() const { return y; }

    private:
        float x, y;
    };

    /**
     * @brief An event that represents a mouse scrolled event.
     *
     * This event is dispatched when the mouse is scrolled.
     */
    class MouseScrolledEvent : public Event
    {
    public:
        /**
         * @brief Creates a new MouseScrolledEvent.
         *
         * This constructor creates a new MouseScrolledEvent with the specified
         * x and y offsets.
         *
         * @param xOffset The x offset of the scroll.
         * @param yOffset The y offset of the scroll.
         */
        MouseScrolledEvent(float xOffset, float yOffset)
            : xOffset(xOffset), yOffset(yOffset)
        {
        }

        inline const char* getName() const override
        {
            return "MouseScrolledEvent";
        }

        std::string toString() const override
        {
            return "MouseScrolledEvent: " + std::to_string(xOffset) + ", " +
                   std::to_string(yOffset);
        }

        /**
         * @brief Gets the x offset of the scroll.
         *
         * @return The x offset of the scroll.
         */
        inline float getXOffset() const { return xOffset; }

        /**
         * @brief Gets the y offset of the scroll.
         *
         * @return The y offset of the scroll.
         */
        inline float getYOffset() const { return yOffset; }

    private:
        float xOffset, yOffset;
    };

    /**
     * @brief An event that represents a mouse button pressed event.
     *
     * This event is dispatched when a mouse button is pressed.
     */
    class MouseButtonPressedEvent : public Event
    {
    public:
        /**
         * @brief Creates a new MouseButtonPressedEvent.
         *
         * This constructor creates a new MouseButtonPressedEvent with the
         * specified button.
         *
         * @param button The button that was pressed.
         */
        MouseButtonPressedEvent(int button) : button(button) {}

        inline const char* getName() const override
        {
            return "MouseButtonPressedEvent";
        }

        std::string toString() const override
        {
            return "MouseButtonPressedEvent: " + std::to_string(button);
        }

        /**
         * @brief Gets the button that was pressed.
         *
         * @return The button that was pressed.
         */
        inline int getButton() const { return button; }

    private:
        // The button that was pressed
        int button;
    };

    /**
     * @brief An event that represents a mouse button released event.
     *
     * This event is dispatched when a mouse button is released.
     */
    class MouseButtonReleasedEvent : public Event
    {
    public:
        /**
         * @brief Creates a new MouseButtonReleasedEvent.
         *
         * This constructor creates a new MouseButtonReleasedEvent with the
         * specified button.
         *
         * @param button The button that was released.
         */
        MouseButtonReleasedEvent(int button) : button(button) {}

        inline const char* getName() const override
        {
            return "MouseButtonReleasedEvent";
        }

        std::string toString() const override
        {
            return "MouseButtonReleasedEvent: " + std::to_string(button);
        }

        /**
         * @brief Gets the button that was released.
         *
         * @return The button that was released.
         */
        inline int getButton() const { return button; }

    private:
        // The button that was released
        int button;
    };
}  // namespace Engine