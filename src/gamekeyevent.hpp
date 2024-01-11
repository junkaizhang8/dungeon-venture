#pragma once

#include <GLFW/glfw3.h>
#include "keyevent.hpp"

class GameKeyEvent : public KeyEvent
{
public:
    GameKeyEvent() = default;
    ~GameKeyEvent() override = default;
    void resetKeys() override;
    bool wPressed() { return w; }
    bool sPressed() { return s; }
    bool aPressed() { return a; }
    bool dPressed() { return d; }

private:
    bool w = false;
    bool s = false;
    bool a = false;
    bool d = false;
    bool left = false;
    bool right = false;
    void handleKeyEvent(GLFWwindow *window, int key, int action) override;
};