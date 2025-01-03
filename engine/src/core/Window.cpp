#include "Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <utils/EngineDebug.h>

#include <functional>

#include "events/ApplicationEvent.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

#define GET_WINDOW_DATA(window) *(WindowData*)glfwGetWindowUserPointer(window)

namespace Engine
{
    static bool glfwInitialized = false;
    static bool glewInitialized = false;

    static void glfwErrorCallback(int error, const char* description)
    {
        GL_LOG_ERROR("(%d): %s", error, description);
    }

    Window::Window(const std::string& title, unsigned int width,
                   unsigned int height)
    {
        data.title = title;
        data.width = width;
        data.height = height;

        if (!glfwInitialized)
        {
            int success = glfwInit();
            ENGINE_ASSERT(success);

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            glfwSetErrorCallback(glfwErrorCallback);

            glfwInitialized = true;
        }

        window = glfwCreateWindow(data.width, data.height, data.title.c_str(),
                                  nullptr, nullptr);
        ENGINE_ASSERT(window);
        glfwMakeContextCurrent(window);

        if (!glewInitialized)
        {
            int status = glewInit();
            ENGINE_ASSERT(status == GLEW_OK);
            glewInitialized = true;
        }

        // Set the user pointer to the WindowData struct to allow access to the
        // WindowData struct from GLFW callbacks
        glfwSetWindowUserPointer(window, &data);
        // Enable VSync
        setVSync(true);

        // Set GLFW callbacks
        glfwSetWindowSizeCallback(window,
                                  [](GLFWwindow* window, int width, int height)
                                  {
                                      WindowData& data =
                                          GET_WINDOW_DATA(window);
                                      data.width = width;
                                      data.height = height;
                                      WindowResizeEvent event(width, height);
                                      data.eventCallback(event);
                                  });

        glfwSetWindowCloseCallback(window,
                                   [](GLFWwindow* window)
                                   {
                                       WindowData& data =
                                           GET_WINDOW_DATA(window);
                                       WindowCloseEvent event;
                                       data.eventCallback(event);
                                   });

        glfwSetKeyCallback(
            window,
            [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                WindowData& data = GET_WINDOW_DATA(window);
                switch (action)
                {
                    case GLFW_PRESS:
                    {
                        KeyPressedEvent event(key, false);
                        data.eventCallback(event);
                        break;
                    }
                    case GLFW_RELEASE:
                    {
                        KeyReleasedEvent event(key);
                        data.eventCallback(event);
                        break;
                    }
                    case GLFW_REPEAT:
                    {
                        KeyPressedEvent event(key, true);
                        data.eventCallback(event);
                        break;
                    }
                }
            });

        glfwSetCursorPosCallback(
            window,
            [](GLFWwindow* window, double xpos, double ypos)
            {
                WindowData& data = GET_WINDOW_DATA(window);
                MouseMovedEvent event(xpos, ypos);
                data.eventCallback(event);
            });

        glfwSetScrollCallback(
            window,
            [](GLFWwindow* window, double xoffset, double yoffset)
            {
                WindowData& data = GET_WINDOW_DATA(window);
                MouseScrolledEvent event(xoffset, yoffset);
                data.eventCallback(event);
            });

        glfwSetMouseButtonCallback(
            window,
            [](GLFWwindow* window, int button, int action, int mods)
            {
                WindowData& data = GET_WINDOW_DATA(window);
                switch (action)
                {
                    case GLFW_PRESS:
                    {
                        MouseButtonPressedEvent event(button);
                        data.eventCallback(event);
                        break;
                    }
                    case GLFW_RELEASE:
                    {
                        MouseButtonReleasedEvent event(button);
                        data.eventCallback(event);
                        break;
                    }
                }
            });

        // Enable point size
        glEnable(GL_PROGRAM_POINT_SIZE);
    }

    Window::~Window() { destroy(); }

    void Window::onUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    void Window::destroy()
    {
        if (window) glfwDestroyWindow(window);
    }
}  // namespace Engine