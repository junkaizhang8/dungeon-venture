#include <iostream>
#include "cursor.hpp"

static void mouseButtonCallback(GLFWwindow *w, int button, int action, int mods)
{
    Cursor::checkMouseButtonEvent(w, button, action, mods);
}

GLFWwindow *Cursor::window = nullptr;
int Cursor::pixelScale = 0;
bool Cursor::leftButtonPressed = false;
bool Cursor::rightButtonPressed = false;

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
            leftButtonPressed = true;
        } else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            rightButtonPressed = true;
        }
    }

    if (action == GLFW_RELEASE)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            leftButtonPressed = false;
        } else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            rightButtonPressed = false;
        }
    }
}

void Cursor::setMouseButtonCallback(GLFWwindow *w)
{
    glfwSetMouseButtonCallback(w, mouseButtonCallback);
    window = w;
}

void Cursor::setPixelScale(int scale)
{
    pixelScale = scale;
}

// Get cursor position in standard coordinate scale
void Cursor::getCursorPos(double *x, double *y)
{
    glfwGetCursorPos(window, x, y);
}

// Get cursor position in terms of the pixel scale
// (e.g. (100, 200) in terms of the standard coordinate scale
// becomes (10, 20) using a pixel scale of 10)
void Cursor::getScaledCursorPos(double *x, double *y)
{
    double standardX;
    double standardY;
    glfwGetCursorPos(window, &standardX, &standardY);
    *x = (int)(standardX / pixelScale);
    *y = (int)(standardY / pixelScale);
}