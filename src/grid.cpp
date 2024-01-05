#include <iostream>
#include "grid.hpp"

void Grid::init(int xpos, int ypos, int width, int height, std::shared_ptr<Renderer> gridRenderer)
{
    x = xpos;
    y = ypos;
    gridWidth = width;
    gridHeight = height;
    renderer = gridRenderer;
}

void Grid::drawGrid()
{
    int rowAndColumnNum = 10;
    double gridlineWidth = 0.2;
    double gridlineWidthHalf = gridlineWidth / 2;
    int cellWidth = gridWidth / rowAndColumnNum;
    int cellHeight = gridHeight / rowAndColumnNum;

    renderer.get()->setColour(255, 255, 255);
    for (int i = 0; i <= rowAndColumnNum; i++)
    {
        renderer.get()->drawRect(cellWidth * i - gridlineWidthHalf + x, y, gridlineWidth, gridHeight);
        renderer.get()->drawRect(x, cellHeight * i - gridlineWidthHalf + y, gridWidth, gridlineWidth);
    }
}