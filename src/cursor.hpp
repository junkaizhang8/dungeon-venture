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
    static void getCursorPos(double *x, double *y) { glfwGetCursorPos(window, x, y); }
    static void getScaledCursorPos(int &x, int &y);
    static bool leftButtonDown() { return left; }
    static bool rightButtonDown() { return right; }
    static bool mouseButtonDown() { return buttonDown; }
    static void getScaledOnLeftClickPos(int &x, int &y);
    static void getScaledOnRightClickPos(int &x, int &y);

private:
    static GLFWwindow *window;
    static int pixelScale;
    static bool left;
    static bool right;
    static bool buttonDown;
    static int scaledOnLeftClickX;
    static int scaledOnLeftClickY;
    static int scaledOnRightClickX;
    static int scaledOnRightClickY;
};