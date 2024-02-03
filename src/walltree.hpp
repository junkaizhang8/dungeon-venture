#pragma once

#include <memory>
#include <stack>
#include "wall.hpp"
#include "renderer.hpp"

// Forward declaration to avoid compiler error

class WallTreeIterator;
class WallTreePreOrderIterator;

// BST for storing walls sorted by their ids.
class WallTree
{
friend class WallTreeIterator;
friend class WallTreePreOrderIterator;

public:
    WallTree() = default;
    ~WallTree() = default;
    WallTreeIterator begin();
    WallTreeIterator end();
    WallTreePreOrderIterator preOrderBegin();
    WallTreePreOrderIterator preOrderEnd();
    void insert(const std::shared_ptr<Wall> &newWall);
    void remove(wall_id id);
    std::shared_ptr<Wall> search(wall_id id) const;

    // Debugging method
    // void draw();

private:
    struct WallNode
    {
        WallNode(const std::shared_ptr<Wall> &newWall)
            : wall(newWall) {}
        std::shared_ptr<Wall> wall;
        std::unique_ptr<WallNode> left;
        std::unique_ptr<WallNode> right;
        WallNode *parent = nullptr;
        int height = 1;
    };
    
    std::unique_ptr<WallNode> root;
    void insert(std::unique_ptr<WallNode> &node, WallNode *const prevNode, const std::shared_ptr<Wall> &newWall);
    std::unique_ptr<WallNode> remove(std::unique_ptr<WallNode> &node, WallNode *const prevNode, wall_id id);
    std::shared_ptr<Wall> search(const std::unique_ptr<WallNode> &node, wall_id id) const;
    std::shared_ptr<Wall> findMin(const std::unique_ptr<WallNode> &node) const;
    void rotateLeft(std::unique_ptr<WallNode> &pivot);
    void rotateRight(std::unique_ptr<WallNode> &pivot);
    void rebalance(std::unique_ptr<WallNode> &node);
    int getHeight(const std::unique_ptr<WallNode> &node) const { return (node) ? node->height : 0; }
    int getBalance(const std::unique_ptr<WallNode> &node) const { return (node) ?
                                                            getHeight(node->left) - getHeight(node->right) :
                                                            0; }
    int max(int a, int b) { return (a >= b) ? a : b; }

    // Debugging method
    // void draw(const std::unique_ptr<WallNode> &node);

};

class WallTreeIterator
{
friend class WallTree;

public:
    WallTreeIterator() = default;
    ~WallTreeIterator() = default;
    const Wall *operator*() const { return (node) ? node->wall.get() : nullptr; }
    bool operator==(const WallTreeIterator &rhs) const { return **this == *rhs; }
    bool operator!=(const WallTreeIterator &rhs) const { return **this != *rhs; }
    WallTreeIterator &operator++();
    WallTreeIterator operator++(int);
    
private:
    const WallTree::WallNode *node;

    WallTreeIterator(const WallTree::WallNode *n)
        : node(n) {}
};

class WallTreePreOrderIterator
{
friend class WallTree;

public:
    WallTreePreOrderIterator() = default;
    ~WallTreePreOrderIterator() = default;
    const Wall *operator*() const { return (node) ? node->wall.get() : nullptr; }
    bool operator==(const WallTreePreOrderIterator &rhs) const { return **this == *rhs; }
    bool operator!=(const WallTreePreOrderIterator &rhs) const { return **this != *rhs; }
    WallTreePreOrderIterator &operator++();
    WallTreePreOrderIterator operator++(int);
    
private:
    const WallTree::WallNode *node;
    const WallTree *tree;
    std::stack<WallTree::WallNode *> nodeStack;

    WallTreePreOrderIterator(const WallTree::WallNode *n, const WallTree *t)
        : node(n), tree(t) {}
};