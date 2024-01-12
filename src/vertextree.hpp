#pragma once

#include <memory>
#include "vertex.hpp"
#include "renderer.hpp"

// K-d tree implementation for storing vertices, sorted by their Cartesian coordinates
class VertexTree
{
public:
    VertexTree() = default;
    ~VertexTree() = default;
    void insert(const std::shared_ptr<Vertex> &newVertex);
    std::shared_ptr<Vertex> search(const int coords[2]);
    std::shared_ptr<Vertex> proximitySearch(const int coords[2], double dist);
    void drawVertices(std::shared_ptr<Renderer> &renderer, int gridLeft, int gridRight,\
                      int gridTop, int gridBottom);

private:
    struct VertexNode
    {
        VertexNode(const std::shared_ptr<Vertex> newVertex) : vertex(newVertex) {}
        std::shared_ptr<Vertex> vertex;
        std::unique_ptr<VertexNode> left;
        std::unique_ptr<VertexNode> right;
    };
    
    std::unique_ptr<VertexNode> root;
    void insert(std::unique_ptr<VertexNode> &node, const std::shared_ptr<Vertex> &newVertex,\
                unsigned depth);
    std::shared_ptr<Vertex> search(const std::unique_ptr<VertexNode> &node, const int coords[2],\
                                   unsigned depth);
    std::shared_ptr<Vertex> proximitySearch(const std::unique_ptr<VertexNode> &node,\
                                            const int coords[2], double distSquared, unsigned depth);
    void drawVertices(std::unique_ptr<VertexNode> &node, std::shared_ptr<Renderer> &renderer,\
                      int gridLeft, int gridRight, int gridTop, int gridBottom);
};