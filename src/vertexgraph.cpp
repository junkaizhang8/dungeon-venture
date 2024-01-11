#include "vertex.hpp"
#include "vertexgraph.hpp"

void VertexGraph::insertMapping(Wall &wall)
{
    vertex_id startVertexId = wall.getStartVertex().get()->getId();
    vertex_id endVertexId = wall.getEndVertex().get()->getId();

    map[startVertexId][endVertexId] = wall.getId();
}