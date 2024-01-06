#pragma once

#include <GLFW/glfw3.h>
#include "window.hpp"

class Cursor
{
public:
    Cursor() = default;
    ~Cursor() = default;
    static void checkMouseButtonEvent(GLFWwindow *w, int button, int action, int mods);
    static void setMouseButtonCallback(GLFWwindow *w);
    static void setPixelScale(int scale);
    static void getCursorPos(double *x, double *y);
    static void getScaledCursorPos(double *x, double *y);

private:
    static GLFWwindow *window;
    static int pixelScale;
    static bool leftButtonPressed;
    static bool rightButtonPressed;
};