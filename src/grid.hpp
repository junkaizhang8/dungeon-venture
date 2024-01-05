#pragma once

#include <iostream>
#include "renderer.hpp"
#include "modemenu.hpp"

#define DEFAULT_GRID_WIDTH 150
#define DEFAULT_GRID_HEIGHT 150
#define DEFAULT_GRID_X 0
#define DEFAULT_GRID_Y DEFAULT_MODE_MENU_HEIGHT

class Grid
{
public:
    Grid() = default;
    ~Grid() {};
    void init(int xpos, int ypos, int width, int height, std::shared_ptr<Renderer> gridRenderer);
    void drawGrid();

private:
    int x;
    int y;
    int gridWidth;
    int gridHeight;
    std::shared_ptr<Renderer> renderer;
};