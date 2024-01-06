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

int Editor::init(int width, int height, int pixelScale, const char *title)
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

    renderer.get()->init(0.0, (double)scaledWidth, (double)scaledHeight, 0.0, pixelScale);

    KeyEvent::setKeyEvent(&keys);
    KeyEvent::setKeyCallback(window);

    Cursor::setMouseButtonCallback(window);
    Cursor::setPixelScale(pixelScale);

    grid.init(DEFAULT_GRID_LEFT, DEFAULT_GRID_TOP, DEFAULT_GRID_WIDTH, DEFAULT_GRID_HEIGHT, renderer);

    return INIT_SUCCESS;
}

void Editor::run()
{
    update();
    display();
}

void Editor::update()
{
    // Cursor::getScaledCursorPos(&x, &y);
    // std::cout << x << " " << y << std::endl;
    
    // Wall mode
    clock.updateClock();
}

void Editor::display()
{   
    if (clock.updateFrame())
    {
        renderer.get()->clearScreen();
        grid.drawGrid();
        if (keys.fPressed())
        {
            grid.drawNewWall();
        } else
        {
            grid.cancelNewWall();
        }
        renderer.get()->setColour(0, 255, 0);
        double test[4][2] = {{20, 30}, {17, 32}, {26, 32}, {26, 28}};
        renderer.get()->drawQuad(test);
        renderer.get()->drawHollowCircle(10, 30, 1);
        glfwSwapBuffers(window);
    }
    glfwPollEvents();
}

bool Editor::isRunning()
{
    return !glfwWindowShouldClose(window);
}

void Editor::close()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}