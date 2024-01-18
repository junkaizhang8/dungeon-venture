#include "vertex.hpp"
#include "renderer.hpp"

vertex_id Vertex::largestId = 0;

void Vertex::drawObj(const Renderer *const renderer, int gridLeft, int gridRight, int gridTop, int gridBottom) const
{
    if (renderer == nullptr)
    {
        return;
    }

    int coords[2] = {x, y};

    getObjPosRelativeToWindow(coords, gridLeft, gridRight, gridTop, gridBottom);

    renderer->drawFilledCircle(coords[0], coords[1], GRID_VERTEX_RADIUS, VERTEX_COLOUR);
}

void Vertex::drawObjSelected(const Renderer *renderer, int gridLeft, int gridRight, int gridTop, int gridBottom) const
{
    if (renderer == nullptr)
    {
        return;
    }

    int coords[2] = {x, y};

    getObjPosRelativeToWindow(coords, gridLeft, gridRight, gridTop, gridBottom);

    renderer->drawFilledCircle(coords[0], coords[1], GRID_VERTEX_RADIUS, VERTEX_SELECTED_COLOUR);
}

void Vertex::getCoords(int coords[2]) const
{
    coords[0] = x;
    coords[1] = y;
}