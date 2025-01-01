#include "Input.h"

#include "Application.h"
#include "utils/EngineDebug.h"

namespace Engine
{
    bool Input::isKeyPressed(int key)
    {
        GLFWwindow* window =
            Application::getInstance().getWindow().getNativeWindow();

        ENGINE_ASSERT(window, "Window is null");

        int state = glfwGetKey(window, key);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::isMouseButtonPressed(int button)
    {
        GLFWwindow* window =
            Application::getInstance().getWindow().getNativeWindow();

        ENGINE_ASSERT(window, "Window is null");

        int state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    std::pair<float, float> Input::getMousePosition()
    {
        GLFWwindow* window =
            Application::getInstance().getWindow().getNativeWindow();

        ENGINE_ASSERT(window, "Window is null");

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return {xpos, ypos};
    }

    float Input::getMouseX() { return getMousePosition().first; }

    float Input::getMouseY() { return getMousePosition().second; }
}  // namespace Engine