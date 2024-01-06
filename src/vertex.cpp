#include "vertex.hpp"

Vertex::Vertex(int xpos, int ypos) : x(xpos), y(ypos) {}

void Vertex::getCoords(double coords[2])
{
    coords[0] = x;
    coords[1] = y;
}