#pragma once

#include "keyevent.hpp"

class EditorKeyEvent : public KeyEvent
{
public:
    EditorKeyEvent() = default;
    ~EditorKeyEvent() override = default;
    void resetKeys() override;
    bool qPressed() { return q; }
    bool fPressed() { return f; }

private:
    bool q = false;
    bool f = false;

    void handleKeyEvent(GLFWwindow *window, int key, int action) override;
};