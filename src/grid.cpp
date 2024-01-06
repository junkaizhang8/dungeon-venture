#include <iostream>
#include <cmath>
#include "cursor.hpp"
#include "grid.hpp"

void Grid::init(int xpos, int ypos, int width, int height, std::shared_ptr<Renderer> gridRenderer)
{
    x = xpos;
    y = ypos;
    gridWidth = width;
    gridHeight = height;
    renderer = gridRenderer;
    newWall = std::make_shared<Wall>();
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

void Grid::drawNewWall()
{
    int cursorX;
    int cursorY;

    Cursor::getScaledCursorPos(&cursorX, &cursorY);

    if (Cursor::leftButtonPressed())
    {
        std::shared_ptr<Vertex> vertex = std::make_shared<Vertex>(cursorX, cursorY);

        if (newWall.get()->getStartVertex() == nullptr)
        {
            newWall.get()->setStartVertex(vertex);
        }

        newWall.get()->setEndVertex(vertex);
        renderer.get()->setColour(255, 255, 0);
        drawWall(newWall);
    }

}

void Grid::cancelNewWall()
{

}

void Grid::drawWall(std::shared_ptr<Wall> wall)
{
    double startCoords[2];
    double endCoords[2];

    wall.get()->getStartVertex().get()->getCoords(startCoords);
    wall.get()->getEndVertex().get()->getCoords(endCoords);

    renderer.get()->drawFilledCircle(startCoords[0], startCoords[1], GRID_VERTEX_RADIUS);
    renderer.get()->drawFilledCircle(endCoords[0], endCoords[1], GRID_VERTEX_RADIUS);

    double wallWidth = GRID_WALL_WIDTH;
    double wallWidthHalf = wallWidth / 2;
    double theta = atan((startCoords[1] - endCoords[1]) / (startCoords[0] - endCoords[0]));
    double xOffset = wallWidthHalf * sin(theta);
    double yOffset = wallWidthHalf * cos(theta);

    double vertices[4][2] =
    {
        {startCoords[0] - xOffset, startCoords[1] + yOffset},
        {startCoords[0] + xOffset, startCoords[1] - yOffset},
        {endCoords[0] + xOffset, endCoords[1] - yOffset},
        {endCoords[0] - xOffset, endCoords[1] + yOffset}
    };

    renderer.get()->drawQuad(vertices);
}