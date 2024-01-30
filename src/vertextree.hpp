#pragma once

#include <memory>
#include <stack>
#include "vertex.hpp"
#include "renderer.hpp"

#define X_DIMENSION 0
#define Y_DIMENSION 1
#define NO_VERTEX_ID -1

// Forward declaration to avoid compiler error

class VertexTreeIterator;
class VertexTreePreOrderIterator;

// K-d tree implementation for storing vertices, sorted by their Cartesian coordinates
class VertexTree
{
friend class VertexTreeIterator;
friend class VertexTreePreOrderIterator;

public:
    VertexTree() = default;
    ~VertexTree() = default;
    VertexTreeIterator begin();
    VertexTreeIterator end();
    VertexTreePreOrderIterator preOrderBegin();
    VertexTreePreOrderIterator preOrderEnd();
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
    std::shared_ptr<Vertex> findMin(const std::unique_ptr<VertexNode> &node, int dimension, unsigned depth);
    std::shared_ptr<Vertex> determineMin(const std::shared_ptr<Vertex> &v1, const std::shared_ptr<Vertex> &v2,
                                         const std::shared_ptr<Vertex> &v3, int dimension);
    bool goLeft(const std::unique_ptr<VertexNode> &node, const int coords[2], unsigned depth);
};

class VertexTreeIterator
{
friend class VertexTree;

public:
    VertexTreeIterator() = default;
    ~VertexTreeIterator() = default;
    const Vertex *operator*() const { return (node) ? node->vertex.get() : nullptr; }
    bool operator==(const VertexTreeIterator &rhs) const { return **this == *rhs; }
    bool operator!=(const VertexTreeIterator &rhs) const { return **this != *rhs; }
    VertexTreeIterator &operator++();
    VertexTreeIterator operator++(int);
    
private:
    const VertexTree::VertexNode *node;

    VertexTreeIterator(const VertexTree::VertexNode *n)
        : node(n) {}
};

class VertexTreePreOrderIterator
{
friend class VertexTree;

public:
    VertexTreePreOrderIterator() = default;
    ~VertexTreePreOrderIterator() = default;
    const Vertex *operator*() const { return (node) ? node->vertex.get() : nullptr; }
    bool operator==(const VertexTreePreOrderIterator &rhs) const { return **this == *rhs; }
    bool operator!=(const VertexTreePreOrderIterator &rhs) const { return **this != *rhs; }
    VertexTreePreOrderIterator &operator++();
    VertexTreePreOrderIterator operator++(int);
    
private:
    const VertexTree::VertexNode *node;
    const VertexTree *tree;
    std::stack<VertexTree::VertexNode *> nodeStack;

    VertexTreePreOrderIterator(const VertexTree::VertexNode *n, const VertexTree *t)
        : node(n), tree(t) {}
};