#pragma once

#include <iostream>
#include "vertex.hpp"

class Wall
{
public:
    Wall();
    ~Wall() = default;
    std::shared_ptr<Vertex> getStartVertex() { return startVertex; }
    void setStartVertex(std::shared_ptr<Vertex> v) { startVertex = v; }
    std::shared_ptr<Vertex> getEndVertex() { return endVertex; }
    void setEndVertex(std::shared_ptr<Vertex> v) { endVertex = v; }

private:
    std::shared_ptr<Vertex> startVertex;
    std::shared_ptr<Vertex> endVertex;
};