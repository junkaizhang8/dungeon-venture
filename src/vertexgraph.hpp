#pragma once

#include <map>
#include "vertex.hpp"
#include "wall.hpp"

/*
 * Undirected graph storing key-value pairs between a vertex id and
 * a list of all other vertex ids in which they share a common wall.
 * An example of the mapping structure would look like:
 * {
 *  vertexId1 : {vertexId2 : wallId1, vertexId3 : wallId2},
 *  vertexId2 : {vertexId1 : wallId1},
 *  vertexId3 : {vertexId1 : wallId2}
 * }
 */ 
class VertexGraph
{
public:
    VertexGraph() = default;
    ~VertexGraph() = default;
    void insertMapping(Wall &wall);

private:
    std::map<vertex_id, std::map<vertex_id, wall_id>> map;
};