#pragma once

#include <memory>
#include "wall.hpp"
#include "renderer.hpp"

// Forward declaration to avoid compiler error
class WallTreeIterator;

// BST for storing walls sorted by their ids.
class WallTree
{
friend class WallTreeIterator;

public:
    WallTree() = default;
    ~WallTree() = default;
    WallTreeIterator begin();
    WallTreeIterator end();
    void insert(const std::shared_ptr<Wall> &newWall);
    void remove(wall_id id);
    std::shared_ptr<Wall> search(wall_id id) const;

private:
    struct WallNode
    {
        WallNode(const std::shared_ptr<Wall> &newWall)
            : wall(newWall) {}
        std::shared_ptr<Wall> wall;
        std::unique_ptr<WallNode> left;
        std::unique_ptr<WallNode> right;
        WallNode *parent = nullptr;
    };
    
    std::unique_ptr<WallNode> root;
    void insert(std::unique_ptr<WallNode> &node, const std::shared_ptr<Wall> &newWall);
    std::unique_ptr<WallNode> remove(std::unique_ptr<WallNode> &node, wall_id id);
    std::shared_ptr<Wall> search(const std::unique_ptr<WallNode> &node, wall_id id) const;
    void drawWalls(const std::unique_ptr<WallNode> &node, const Renderer *const renderer,\
                   int gridLeft, int gridRight, int gridTop, int gridBottom) const;
    std::shared_ptr<Wall> findMin(const std::unique_ptr<WallNode> &node) const;
};

class WallTreeIterator
{
friend class WallTree;

public:
    WallTreeIterator() = default;
    ~WallTreeIterator() = default;
    const Wall *operator*() const { return (node) ? node->wall.get() : nullptr; }
    bool operator==(const WallTreeIterator &rhs) { return **this == *rhs; }
    bool operator!=(const WallTreeIterator &rhs) { return **this != *rhs; }
    WallTreeIterator &operator++();
    WallTreeIterator operator++(int);
    
private:
    const WallTree::WallNode *node;
    const WallTree *tree;

    WallTreeIterator(const WallTree::WallNode *n, const WallTree *t)
        : node(n), tree(t) {}
};