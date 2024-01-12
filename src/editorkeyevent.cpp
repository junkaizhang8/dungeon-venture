#include <GLFW/glfw3.h>
#include "keyevent.hpp"
#include "editorkeyevent.hpp"
#include <iostream>

void EditorKeyEvent::resetKeys()
{
    q = false;
    f = false;
}

// Key events for editor window
// q - Cursor mode
// f - Wall mode
void EditorKeyEvent::handleKeyEvent(GLFWwindow *window, int key, int action)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case (GLFW_KEY_Q):
            if (q)
            {
                resetKeys();
            }
            else
            {
                resetKeys();
                q = true;
            }
            break;
        case (GLFW_KEY_F):
            if (f)
            {
                resetKeys();
            }
            else
            {
                resetKeys();
                f = true;
            }
            break;
        default:
            break;
        }
    }
}