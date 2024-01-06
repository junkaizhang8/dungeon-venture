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
    static void setPixelScale(int scale) { pixelScale = scale; }
    
    // Get cursor position in standard coordinates
    static void getCursorPos(double *x, double *y) { glfwGetCursorPos(window, x, y); }
    static void getScaledCursorPos(int *x, int *y);
    static bool leftButtonPressed() { return left; }
    static bool rightButtonPressed() { return right; }

private:
    static GLFWwindow *window;
    static int pixelScale;
    static bool left;
    static bool right;
};