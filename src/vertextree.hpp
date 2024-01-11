#pragma once

#include <memory>
#include "vertex.hpp"

// K-d tree implementation for storing vertices, sorted by their Cartesian coordinates
class VertexTree
{
public:
    VertexTree() = default;
    ~VertexTree() = default;
    void insert(const std::shared_ptr<Vertex> &newVertex);
    std::shared_ptr<Vertex> search(const int coords[2]);
    std::shared_ptr<Vertex> proximitySearch(const int coords[2], double dist);

private:
    struct VertexNode
    {
        VertexNode(const std::shared_ptr<Vertex> newVertex) : vertex(newVertex) {}
        std::shared_ptr<Vertex> vertex;
        std::shared_ptr<VertexNode> left;
        std::shared_ptr<VertexNode> right;
    };
    
    std::shared_ptr<VertexNode> root;
    void insert(std::shared_ptr<VertexNode> &node, const std::shared_ptr<Vertex> &newVertex,\
                unsigned depth);
    std::shared_ptr<Vertex> search(const std::shared_ptr<VertexNode> &node, const int coords[2],\
                                   unsigned depth);
    std::shared_ptr<Vertex> proximitySearch(const std::shared_ptr<VertexNode> &node,\
                                            const int coords[2], double distSquared, unsigned depth);
};