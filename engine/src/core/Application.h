#pragma once

#include <memory>
#include <string>

#include "Layer.h"
#include "Window.h"
#include "events/ApplicationEvent.h"
#include "events/Event.h"

namespace Engine
{
    /**
     * @brief A class that represents an application.
     *
     * This class represents an application that uses the engine. It is a
     * singleton class that manages the main loop of the application.
     */
    class Application
    {
    public:
        /**
         * @brief Constructs a new Application object.
         *
         * This constructor creates a new Application object. It also contains
         * basic event handling for the WindowCloseEvent. Users can override the
         * onWindowClose method in subclasses to allow for custom behavior when
         * the window is closed.
         *
         * @param title The title of the application window. Defaults to "App".
         * @param width The width of the application window. Defaults to 1280.
         * @param height The height of the application window. Defaults to 720.
         */
        Application(const std::string& title = "App", unsigned int width = 1280,
                    unsigned int height = 720);

        /**
         * @brief Destroys the Application object.
         *
         * This destructor destroys the Application object and frees any
         * resources associated with it.
         */
        virtual ~Application() = default;

        /**
         * @brief Runs the application.
         *
         * This method runs the application by starting the main loop.
         */
        void run();

        /**
         * @brief Called when an event is received.
         *
         * This method is called when an event is received. The default
         * implementation dispatches the event to the event dispatcher.
         * Subclasses can override this method to handle events.
         *
         * @param event The event received.
         */
        void onEvent(Event& event);

        /**
         * @brief Pushes a layer onto the layer stack.
         *
         * This method pushes a layer onto the layer stack.
         *
         * @param layer The layer to push onto the layer stack.
         */
        void pushLayer(Layer* layer);

        /**
         * @brief Pops a layer from the layer stack.
         *
         * This method pops a layer from the layer stack.
         *
         * @param layer The layer to pop from the layer stack.
         */
        void popLayer(Layer* layer);

        /**
         * @brief Gets the window of the application.
         *
         * This method returns the window of the application.
         *
         * @return The window of the application.
         */
        inline Window& getWindow() { return *window; }

        /**
         * @brief Gets the instance of the Application class.
         *
         * This method returns the instance of the Application class.
         *
         * @return The instance of the Application class.
         */
        inline static Application& getInstance() { return *instance; }

    protected:
        // The event dispatcher.
        EventDispatcher dispatcher;

        void onWindowResize(WindowResizeEvent& event);

        /**
         * @brief Called when the window is closed.
         *
         * This method is called when the window is closed.
         *
         * @param event The WindowCloseEvent.
         */
        void onWindowClose(WindowCloseEvent& event);

    private:
        // The instance of the Application class
        static Application* instance;
        // The window of the application
        std::unique_ptr<Window> window;
        // The layer stack of the application
        LayerStack layerStack;
        // Flag indicating whether the application is running
        bool running = false;
        // The time of last frame
        float lastFrameTime = 0.0f;

        void eventLoop();
    };
}  // namespace Engine