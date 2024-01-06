#include <GLFW/glfw3.h>
#include "keyevent.hpp"

static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    KeyEvent::checkKeyEvent(window, key, scancode, action, mods);
}

KeyEvent *KeyEvent::keyEvent = nullptr;

void KeyEvent::checkKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (!keyEvent || !window)
    {
        return;
    }

    if (!windowShouldClose(window, key, action))
    {
        keyEvent->handleKeyEvent(window, key, action);
    }
}

void KeyEvent::setKeyEvent(KeyEvent *newKeyEvent)
{
    if (newKeyEvent)
    {
        keyEvent = newKeyEvent;
    }
}

void KeyEvent::setKeyCallback(GLFWwindow *window) {
    glfwSetKeyCallback(window, keyCallback);
}

bool KeyEvent::windowShouldClose(GLFWwindow *window, int key, int action)
{
    if (!window)
    {
        return false;
    }

    // ESC to quit
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return true;
    }

    return false;
}