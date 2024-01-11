#pragma once

#include <memory>
#include "vertex.hpp"

typedef int wall_id;

class Wall
{
public:
    Wall() : id(largestId++) {}
    Wall(wall_id wallId, std::shared_ptr<Vertex> start, std::shared_ptr<Vertex> end) : id(wallId),\
                                                                                       startVertex(start),\
                                                                                       endVertex(end) {}
    ~Wall() = default;
    void resetVertices();
    wall_id getId() { return id; }
    void setId(wall_id newId) { id = newId; }
    std::shared_ptr<Vertex> getStartVertex() { return startVertex; }
    void setStartVertex(std::shared_ptr<Vertex> v) { startVertex = v; }
    std::shared_ptr<Vertex> getEndVertex() { return endVertex; }
    void setEndVertex(std::shared_ptr<Vertex> v) { endVertex = v; }

private:
    static wall_id largestId;
    wall_id id;
    std::shared_ptr<Vertex> startVertex;
    std::shared_ptr<Vertex> endVertex;
};