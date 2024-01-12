#include "wall.hpp"
#include "renderer.hpp"

wall_id Wall::largestId = 0;

void Wall::drawObj(std::shared_ptr<Renderer> renderer, int gridLeft, int gridRight, int gridTop, int gridBottom)
{
    if (renderer == nullptr)
    {
        return;
    }

    int startCoords[2] = {};
    int endCoords[2] = {};

    startVertex.get()->getCoords(startCoords);
    endVertex.get()->getCoords(endCoords);
    getObjPosRelativeToWindow(startCoords, gridLeft, gridRight, gridTop, gridBottom);
    getObjPosRelativeToWindow(endCoords, gridLeft, gridRight, gridTop, gridBottom);

    double wallWidth = GRID_WALL_WIDTH;
    double wallWidthHalf = wallWidth / 2;
    double theta = atan((double)(startCoords[1] - endCoords[1]) / (double)(startCoords[0] - endCoords[0]));
    double xOffset = wallWidthHalf * sin(theta);
    double yOffset = wallWidthHalf * cos(theta);

    double vertices[4][2] =
    {
        {startCoords[0] - xOffset, startCoords[1] + yOffset},
        {startCoords[0] + xOffset, startCoords[1] - yOffset},
        {endCoords[0] + xOffset, endCoords[1] - yOffset},
        {endCoords[0] - xOffset, endCoords[1] + yOffset}
    };

    renderer.get()->drawQuad(vertices, WALL_COLOUR);
}

void Wall::drawObjSelected(std::shared_ptr<Renderer> renderer, int gridLeft, int gridRight, int gridTop, int gridBottom)
{
    if (renderer == nullptr)
    {
        return;
    }

    int startCoords[2] = {};
    int endCoords[2] = {};

    startVertex.get()->getCoords(startCoords);
    endVertex.get()->getCoords(endCoords);
    getObjPosRelativeToWindow(startCoords, gridLeft, gridRight, gridTop, gridBottom);
    getObjPosRelativeToWindow(endCoords, gridLeft, gridRight, gridTop, gridBottom);

    double wallWidth = GRID_WALL_WIDTH;
    double wallWidthHalf = wallWidth / 2;
    double theta = atan((double)(startCoords[1] - endCoords[1]) / (double)(startCoords[0] - endCoords[0]));
    double xOffset = wallWidthHalf * sin(theta);
    double yOffset = wallWidthHalf * cos(theta);

    double vertices[4][2] =
    {
        {startCoords[0] - xOffset, startCoords[1] + yOffset},
        {startCoords[0] + xOffset, startCoords[1] - yOffset},
        {endCoords[0] + xOffset, endCoords[1] - yOffset},
        {endCoords[0] - xOffset, endCoords[1] + yOffset}
    };

    renderer.get()->drawQuad(vertices, WALL_SELECTED_COLOUR);
}

void Wall::resetVertices()
{
    startVertex = nullptr;
    endVertex = nullptr;
}