#include <iostream>
#include <GLFW/glfw3.h>
#include "window.hpp"
#include "game.hpp"
#include "editor.hpp"

int main(int argc, char *argv[])
{
    Editor editor;
    editor.init(DEFAULT_EDITOR_WIDTH, DEFAULT_EDITOR_HEIGHT, DEFAULT_PIXEL_SCALE, "Editor Window");

    while (editor.isRunning())
    {
        editor.run();
    }

    editor.close();

    return EXIT_SUCCESS;
}