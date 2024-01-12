#include "vertex.hpp"
#include "renderer.hpp"

vertex_id Vertex::largestId = 0;

void Vertex::drawObj(std::shared_ptr<Renderer> renderer, int gridLeft, int gridRight, int gridTop, int gridBottom)
{
    if (renderer == nullptr)
    {
        return;
    }

    int coords[2] = {x, y};

    getObjPosRelativeToWindow(coords, gridLeft, gridRight, gridTop, gridBottom);

    renderer.get()->drawFilledCircle(coords[0], coords[1], GRID_VERTEX_RADIUS, VERTEX_COLOUR);
}

void Vertex::drawObjSelected(std::shared_ptr<Renderer> renderer, int gridLeft, int gridRight, int gridTop, int gridBottom)
{
    if (renderer == nullptr)
    {
        return;
    }

    int coords[2] = {x, y};

    getObjPosRelativeToWindow(coords, gridLeft, gridRight, gridTop, gridBottom);

    renderer.get()->drawFilledCircle(coords[0], coords[1], GRID_VERTEX_RADIUS, VERTEX_SELECTED_COLOUR);
}

void Vertex::getCoords(int coords[2])
{
    coords[0] = x;
    coords[1] = y;
}