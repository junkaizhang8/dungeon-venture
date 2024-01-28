#pragma once

#include <memory>
#include "vertex.hpp"
#include "renderer.hpp"

#define X_DIMENSION 0
#define Y_DIMENSION 1
#define NO_VERTEX_ID -1

// Forward declaration to avoid compiler error
class VertexTreeIterator;

// K-d tree implementation for storing vertices, sorted by their Cartesian coordinates
class VertexTree
{
friend class VertexTreeIterator;

public:
    VertexTree() = default;
    ~VertexTree() = default;
    VertexTreeIterator begin();
    VertexTreeIterator end();
    void insert(const std::shared_ptr<Vertex> &newVertex);
    void remove(int id, const int coords[2]);
    std::shared_ptr<Vertex> search(const int coords[2]) const;
    std::shared_ptr<Vertex> proximitySearch(const int coords[2], double dist) const;

private:
    struct VertexNode
    {
        VertexNode(const std::shared_ptr<Vertex> &newVertex)
            : vertex(newVertex) {}
        std::shared_ptr<Vertex> vertex;
        std::unique_ptr<VertexNode> left;
        std::unique_ptr<VertexNode> right;
        VertexNode *parent = nullptr;
    };

    std::unique_ptr<VertexNode> root;
    void insert(std::unique_ptr<VertexNode> &node, VertexNode *const prevNode,
                const std::shared_ptr<Vertex> &newVertex, unsigned depth);
    std::unique_ptr<VertexNode> remove(std::unique_ptr<VertexNode> &node, VertexNode *const prevNode,
                                       int id, const int coords[2], unsigned depth);
    std::shared_ptr<Vertex> search(const std::unique_ptr<VertexNode> &node, const int coords[2],
                                   unsigned depth) const;
    std::shared_ptr<Vertex> proximitySearch(const std::unique_ptr<VertexNode> &node, const int coords[2],
                                            double distSquared, unsigned depth) const;
    void drawVertices(const std::unique_ptr<VertexNode> &node, const Renderer *const renderer,
                      int gridLeft, int gridRight, int gridTop, int gridBottom) const;
    std::shared_ptr<Vertex> findMin(const std::unique_ptr<VertexNode> &node, int dimension, unsigned depth);
    std::shared_ptr<Vertex> determineMin(const std::shared_ptr<Vertex> &v1, const std::shared_ptr<Vertex> &v2,
                                         const std::shared_ptr<Vertex> &v3, int dimension);
};

class VertexTreeIterator
{
friend class VertexTree;

public:
    VertexTreeIterator() = default;
    ~VertexTreeIterator() = default;
    const Vertex *operator*() const { return (node) ? node->vertex.get() : nullptr; }
    bool operator==(const VertexTreeIterator &rhs) { return **this == *rhs; }
    bool operator!=(const VertexTreeIterator &rhs) { return **this != *rhs; }
    VertexTreeIterator &operator++();
    VertexTreeIterator operator++(int);
    
private:
    const VertexTree::VertexNode *node;
    const VertexTree *tree;

    VertexTreeIterator(const VertexTree::VertexNode *n, const VertexTree *t)
        : node(n), tree(t) {}
};