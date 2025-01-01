#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functional>
#include <string>

#include "events/Event.h"

namespace Engine
{
    /**
     * @brief A class that encapsulates a window.
     *
     * This class provides a simple interface for creating and managing windows.
     */
    class Window
    {
    public:
        // The event handler function type
        using EventHandler = std::function<void(Event&)>;

        /**
         * @brief Constructs a new Window object
         *
         * This constructor creates a new Window object with the specified
         * properties.
         *
         * @param title The title of the window. Defaults to "App"
         * @param width The width of the window. Defaults to 1280
         * @param height The height of the window. Defaults to 720
         *
         */
        Window(const std::string& title = "App", unsigned int width = 1280,
               unsigned int height = 720);

        /**
         * @brief Destroys the Window object
         *
         * This destructor destroys the Window object and frees any resources
         * associated with it.
         */
        ~Window();

        /**
         * @brief Updates the window.
         *
         * This method updates the window by polling for events and swapping the
         * buffers.
         */
        void onUpdate();

        /**
         * @brief Destroys the window.
         *
         * This method destroys the window and frees any resources associated
         * with it.
         */
        void destroy();

        /**
         * @brief Gets the title of the window.
         *
         * This method returns the title of the window.
         *
         * @return The title of the window
         */
        inline const std::string& getTitle() const { return data.title; }

        /**
         * @brief Gets the width of the window.
         *
         * This method returns the width of the window.
         *
         * @return The width of the window
         */
        inline int getWidth() const { return data.width; }

        /**
         * @brief Gets the height of the window.
         *
         * This method returns the height of the window.
         *
         * @return The height of the window
         */
        inline int getHeight() const { return data.height; }

        /**
         * @brief Gets the vsync status of the window.
         *
         * This method returns the vsync status of the window.
         *
         * @return True if vsync is enabled, false otherwise.
         */
        inline bool isVSync() const { return data.vsync; }

        /**
         * @brief Sets the vsync status of the window.
         *
         * This method sets the vsync status of the window.
         *
         * @param enabled True to enable vsync, false to disable it.
         */
        inline void setVSync(bool enabled)
        {
            data.vsync = enabled;
            glfwSwapInterval(data.vsync ? 1 : 0);
        }

        /**
         * @brief Sets the event callback function.
         *
         * This method sets the event callback function to be called when an
         * event is received by the window.
         *
         * @param callback The callback function to be called when an event is
         * received by the window
         */
        inline void setEventCallback(const EventHandler& callback)
        {
            data.eventCallback = callback;
        }

        inline GLFWwindow* getNativeWindow() const { return window; }

    private:
        /**
         * @brief The data associated with the window.
         *
         * This struct contains the data associated with the window.
         */
        struct WindowData
        {
            // The title of the window
            std::string title;
            // The width of the window
            int width;
            // The height of the window
            int height;
            // The vsync status of the window
            bool vsync;
            // The event callback function
            EventHandler eventCallback;
        };

        // The GLFW window
        GLFWwindow* window;
        // The data associated with the window
        WindowData data;
    };
}  // namespace Engine