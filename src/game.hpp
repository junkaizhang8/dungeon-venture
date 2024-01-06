#pragma once

#include <GLFW/glfw3.h>
#include "window.hpp"
#include "game.hpp"
#include "keyevent.hpp"
#include "gamekeyevent.hpp"
#include "player.hpp"

#define DEFAULT_GAME_WIDTH 240
#define DEFAULT_GAME_HEIGHT 160

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

class Game : public Window
{
public:
    Game() = default;
    ~Game() override;
    int init(int width, int height, int pixelScale, const char *title) override;
    void run();
    void update();
    void display() override;
    bool isRunning() override;
    void close() override;

private:
    GameKeyEvent keys;      // Store key press statuses of movement keys
    Player player;
};