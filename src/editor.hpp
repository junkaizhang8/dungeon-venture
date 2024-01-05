#pragma once

#include <GLFW/glfw3.h>
#include "window.hpp"
#include "keyevent.hpp"
#include "editorkeyevent.hpp"
#include "cursor.hpp"
#include "grid.hpp"
#include "modemenu.hpp"
#include "attributesmenu.hpp"

#define DEFAULT_EDITOR_WIDTH DEFAULT_GRID_WIDTH + DEFAULT_ATTRIBUTES_MENU_WIDTH
#define DEFAULT_EDITOR_HEIGHT DEFAULT_ATTRIBUTES_MENU_HEIGHT

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

class Editor : public Window
{
public:
    Editor() = default;
    ~Editor();
    int init(int width, int height, int pixelScale, const char *title) override;
    void run();
    void update();
    void display() override;
    bool isRunning() override;
    void close() override;

private:
    EditorKeyEvent keys;      // Store key press statuses of certain events
    Cursor cursor;
    Grid grid;
};