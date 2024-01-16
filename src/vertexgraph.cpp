#include "vertex.hpp"
#include "vertexgraph.hpp"

// Insert a mapping between the start vertex and end vertex of wall
void VertexGraph::insertMapping(Wall &wall)
{
    vertex_id startVertexId = wall.getStartVertex().get()->getId();
    vertex_id endVertexId = wall.getEndVertex().get()->getId();
    wall_id wallId = wall.getId();

    map[startVertexId][endVertexId] = wallId;
    map[endVertexId][startVertexId] = wallId;
}

/*
 * Modify every v1 node in the graph to be a v2 node.
 * Precondition: v1 and v2 must already exist in the graph
 * Upon exit, modifiedWalls will contain every wall that has been modified
 * and removedWalls will contain every wall that has been removed.
 * modifiedSize and removedSize will give the sizes of modifiedWalls and
 * removedWalls, respectively
 */
void VertexGraph::modifyMapping(vertex_id v1, vertex_id v2, std::vector<wall_id> &modifiedWalls,
                                std::vector<wall_id> &removedWalls)
{
    if (v1 == v2)
    {
        return;
    }

    // If there are no mappings with vertex v1, return
    if (map.find(v1) == map.end())
    {
        return;
    }

    // If there are no mappings with vertex v2, return
    if (map.find(v2) == map.end())
    {
        return;
    }

    // Remove the mapping between v1 and v2 if it exists since they are
    // now the same vertex, so no walls will be connecting them
    if (map.at(v1).find(v2) != map.at(v1).end())
    {
        removedWalls.emplace_back(map[v1][v2]);
    }

    auto &v1Node = map.at(v1);
    auto &v2Node = map.at(v2);

    v1Node.erase(v2);
    v2Node.erase(v1);

    /*
     * Loop through every mapping in v1Node and modify the
     * corresponding mapping key in the map
     * As an example, before loop, map looks like:
     * {
     *  v1 : {u1 : w1, u2 : w2},
     *  v2 : {u3 : w3},
     *  u1 : {v1 : w1},
     *  u2 : {v1 : w2},
     *  u3 : {v2 : w3}
     * }
     * After loop, we expect map to look like:
     * {
     *  v1 : {u1 : w1, u2 : w2},
     *  v2 : {u3 : w3},
     *  u1 : {v2 : w1},
     *  u2 : {v2 : w2},
     *  u3 : {v2 : w3}
     * }
     */
    for (auto mapping : v1Node)
    {
        vertex_id id = mapping.first;

        auto &idMap = map.at(id);

        // If v2 not in idMap, modify v1 to be v2
        // Else, remove v1 from idMap since if we modify it,
        // then there is an overlap with the other wall
        if (idMap.find(v2) == idMap.end())
        {
            modifiedWalls.emplace_back(mapping.second);

            auto node = idMap.extract(v1);

            node.key() = v2;

            idMap.insert(std::move(node));

            map[v2][id] = mapping.second;
        }
        else
        {
            removedWalls.emplace_back(mapping.second);

            idMap.erase(v1);
        }
    }

    // Remove v1 from map as the vertex no longer exists
    map.erase(v1);

    // Remove v2 from map if it is not connected to any other vertex
    if (!v2Node.size())
    {
        map.erase(v2);
    }
}

// Return true if vertexId is a vertex in the graph
// Return false otherwise
bool VertexGraph::contains(vertex_id vertexId)
{
    return (map.find(vertexId) != map.end());
}

// Return true if the mapping between v1 and v2 is in the graph
// Return false otherwise
bool VertexGraph::contains(vertex_id v1, vertex_id v2)
{
    if (map.find(v1) != map.end())
    {
        return (map.at(v1).find(v2) != map.at(v1).end());
    }

    return false;
}