#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <GLFW/glfw3.h>
#include "keyevent.hpp"
#include "editorkeyevent.hpp"
#include "editor.hpp"

Editor::~Editor()
{
    if (window)
    {
        close();
    }
    std::cout << "Editor window destroyed successfully." << std::endl;
}

int Editor::init(int width, int height, int pixelScale, const char *const title)
{
    if (!glfwInit())
    {
        return INIT_ERROR;
    }

    windowWidth = width;
    windowHeight = height;

    int scaledWidth = width * pixelScale;
    int scaledHeight = height * pixelScale;

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(scaledWidth, scaledHeight, title, NULL, NULL);
    if (!window)
    {
        close();
        return INIT_ERROR;
    }

    glfwMakeContextCurrent(window);

    glfwSetWindowUserPointer(window, this);

    glfwSwapInterval(1);

    renderer = std::make_shared<Renderer>();

    renderer->init(0.0, (double)scaledWidth, (double)scaledHeight, 0.0, pixelScale);

    KeyEvent::setKeyEvent(&keys);
    KeyEvent::setKeyCallback(window);

    Cursor::setMouseButtonCallback(window);
    Cursor::setPixelScale(pixelScale);

    mapData = std::make_unique<MapData>();

    grid.init(DEFAULT_GRID_LEFT, DEFAULT_GRID_RIGHT, DEFAULT_GRID_TOP, DEFAULT_GRID_BOTTOM,
              mapData.get(), renderer.get());

    return INIT_SUCCESS;
}

void Editor::update()
{
    clock.updateClock();
}

void Editor::display()
{
    if (clock.updateFrame())
    {
        renderer->clearScreen();
        grid.drawGrid();

        if (keys.qPressed())
        {
            grid.selectMode();
        }
        else
        {
            grid.cancelSelectMode();
        }

        if (keys.fPressed())
        {
            grid.wallMode();
        }
        else
        {
            grid.cancelWallMode();
        }

        grid.drawGridItems();

        glfwSwapBuffers(window);
    }

    glfwPollEvents();
}