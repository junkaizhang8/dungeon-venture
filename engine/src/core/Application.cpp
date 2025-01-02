#include "Application.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "utils/EngineDebug.h"

namespace Engine
{
    Application* Application::instance = nullptr;

    Application::Application(const std::string& title, unsigned int width,
                             unsigned int height)
    {
        if (instance) LOG_ERROR("Application already exists!");

        instance = this;

        window = std::make_unique<Window>(title, width, height);
        running = true;

        // Add a handler for the WindowResizeEvent
        dispatcher.addHandler<WindowResizeEvent>(
            [this](WindowResizeEvent& event) { onWindowResize(event); });

        // Add a handler for the WindowCloseEvent
        dispatcher.addHandler<WindowCloseEvent>([this](WindowCloseEvent& event)
                                                { onWindowClose(event); });

        // Set the event callback for the window
        window->setEventCallback([this](Event& event) { onEvent(event); });
    }

    void Application::run()
    {
        while (running)
        {
            eventLoop();
        }
    }

    void Application::onEvent(Event& event)
    {
        dispatcher.dispatch(event);

        for (auto layer : layerStack) layer->onEvent(event);
    }

    void Application::pushLayer(Layer* layer) { layerStack.pushLayer(layer); }

    void Application::popLayer(Layer* layer) { layerStack.popLayer(layer); }

    void Application::onWindowResize(WindowResizeEvent& event) { eventLoop(); }

    void Application::onWindowClose(WindowCloseEvent& event)
    {
        running = false;
    }

    void Application::eventLoop()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        for (auto layer : layerStack) layer->onUpdate(deltaTime);

        window->onUpdate();
    }
}  // namespace Engine