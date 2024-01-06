#pragma once

#include <iostream>
#include "renderer.hpp"
#include "modemenu.hpp"

#define DEFAULT_GRID_WIDTH 200
#define DEFAULT_GRID_HEIGHT 200
#define DEFAULT_GRID_LEFT 0
#define DEFAULT_GRID_RIGHT DEFAULT_GRID_WIDTH
#define DEFAULT_GRID_TOP 0
#define DEFAULT_GRID_BOTTOM DEFAULT_GRID_HEIGHT

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