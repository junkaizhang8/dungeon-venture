#pragma once

#include "Event.h"

namespace Engine
{
    /**
     * @brief An event that represents a window resize.
     *
     * This event is dispatched when the window is resized.
     */
    class WindowResizeEvent : public Event
    {
    public:
        /**
         * @brief Creates a new WindowResizeEvent.
         *
         * This constructor creates a new WindowResizeEvent with the specified
         * width and height.
         *
         * @param width The width of the window.
         * @param height The height of the window.
         */
        WindowResizeEvent(unsigned int width, unsigned int height)
            : width(width), height(height)
        {
        }

        inline const char* getName() const override
        {
            return "WindowResizeEvent";
        }

        std::string toString() const override
        {
            return "WindowResizeEvent: " + std::to_string(width) + ", " +
                   std::to_string(height);
        }

        /**
         * @brief Gets the width of the window.
         *
         * @return The width of the window.
         */
        inline unsigned int getWidth() const { return width; }

        /**
         * @brief Gets the height of the window.
         *
         * @return The height of the window.
         */
        inline unsigned int getHeight() const { return height; }

    private:
        unsigned int width, height;
    };

    /**
     * @brief An event that represents a window close.
     *
     * This event is dispatched when the window is closed.
     */
    class WindowCloseEvent : public Event
    {
    public:
        inline const char* getName() const override
        {
            return "WindowCloseEvent";
        }
    };
}  // namespace Engine