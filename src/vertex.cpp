#include "vertex.hpp"

vertex_id Vertex::largestId = 0;

void Vertex::getCoords(int coords[2])
{
    coords[0] = x;
    coords[1] = y;
}