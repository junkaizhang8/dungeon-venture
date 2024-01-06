#include <iostream>
#include "cursor.hpp"

static void mouseButtonCallback(GLFWwindow *w, int button, int action, int mods)
{
    Cursor::checkMouseButtonEvent(w, button, action, mods);
}

GLFWwindow *Cursor::window = nullptr;
int Cursor::pixelScale = 0;
bool Cursor::left = false;
bool Cursor::right = false;

void Cursor::checkMouseButtonEvent(GLFWwindow *w, int button, int action, int mods)
{
    if (!w)
    {
        return;
    }

    if (action == GLFW_PRESS)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            left = true;
        } else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            right = true;
        }
    }

    if (action == GLFW_RELEASE)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            left = false;
        } else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            right = false;
        }
    }
}

void Cursor::setMouseButtonCallback(GLFWwindow *w)
{
    glfwSetMouseButtonCallback(w, mouseButtonCallback);
    window = w;
}

// Get cursor position in terms of the pixel scale
// e.g. (100, 200) in terms of the standard coordinate scale
// becomes (10, 20) using a pixel scale of 10
void Cursor::getScaledCursorPos(int *x, int *y)
{
    double standardX;
    double standardY;
    glfwGetCursorPos(window, &standardX, &standardY);
    *x = (int)(standardX / pixelScale);
    *y = (int)(standardY / pixelScale);
}