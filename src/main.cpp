#include <iostream>
#include <GLFW/glfw3.h>
#include "window.hpp"
#include "game.hpp"
#include "editor.hpp"

int main(int argc, char *argv[])
{
    Editor game;
    game.init(DEFAULT_EDITOR_WIDTH, DEFAULT_EDITOR_HEIGHT, DEFAULT_PIXEL_SCALE, "Editor Window");

    while (game.isRunning())
    {
        game.run();
    }

    game.close();

    return EXIT_SUCCESS;
}