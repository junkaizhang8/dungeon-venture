#pragma once

#include <GLFW/glfw3.h>
#include "keyevent.hpp"

class GameKeyEvent : public KeyEvent
{
public:
    GameKeyEvent() = default;
    ~GameKeyEvent() override = default;
    void resetKeys() override;
    bool wDown() { return w; }
    bool sDown() { return s; }
    bool aDown() { return a; }
    bool dDown() { return d; }

private:
    bool w = false;
    bool s = false;
    bool a = false;
    bool d = false;
    bool left = false;
    bool right = false;
    void handleKeyEvent(GLFWwindow *window, int key, int action) override;
};