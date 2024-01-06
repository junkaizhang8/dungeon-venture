#pragma once

#include <GLFW/glfw3.h>

class KeyEvent
{
public:
    KeyEvent() = default;
    virtual ~KeyEvent() = default;
    virtual void resetKeys() = 0;
    static void checkKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void setKeyEvent(KeyEvent *newKeyEvent) { keyEvent = newKeyEvent; }
    static void setKeyCallback(GLFWwindow *window);
    static bool windowShouldClose(GLFWwindow *window, int key, int action);

protected:
    static KeyEvent *keyEvent;
    virtual void handleKeyEvent(GLFWwindow *window, int key, int action) = 0;
};