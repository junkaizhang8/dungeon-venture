#pragma once

#include <memory>
#include <map>
#include "vertex.hpp"
#include "wall.hpp"

typedef std::unordered_map<vertex_id, std::unordered_map<vertex_id, wall_id>> Graph;

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
    void insertMapping(const Wall &wall);
    void modifyMapping(vertex_id v1, vertex_id v2, std::vector<wall_id> &modifiedWalls,
                       std::vector<wall_id> &removedWalls);
    bool contains(vertex_id vertexId) const;
    bool contains(vertex_id v1, vertex_id v2) const;

private:
    Graph map;
};