#include "wall.hpp"

wall_id Wall::largestId = 0;

void Wall::resetVertices()
{
    startVertex = nullptr;
    endVertex = nullptr;
}