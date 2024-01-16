#include <memory>
#include <iostream>
#include "walltree.hpp"

// Insert newWall into the tree.
void WallTree::insert(const std::shared_ptr<Wall> &newWall)
{
    insert(root, newWall);
}

void WallTree::remove(wall_id id)
{
    root = remove(root, id);
}

std::shared_ptr<Wall> WallTree::search(wall_id id) const
{
    return search(root, id);
}

void WallTree::drawWalls(const Renderer *renderer, int gridLeft, int gridRight, int gridTop, int gridBottom)
{
    drawWalls(root, renderer, gridLeft, gridRight, gridTop, gridBottom);
}

void WallTree::insert(std::unique_ptr<WallNode> &node, const std::shared_ptr<Wall> &newWall)
{
    if (node == nullptr)
    {
        node = std::make_unique<WallNode>(newWall);
        return;
    }

    insert((newWall.get()->getId() <= node.get()->wall.get()->getId()) ? node.get()->left : node.get()->right, newWall);
}

std::unique_ptr<WallTree::WallNode> WallTree::remove(std::unique_ptr<WallNode> &node, wall_id id)
{
    if (node == nullptr)
    {
        return nullptr;
    }

    if (id < node.get()->wall.get()->getId())
    {
        node.get()->left = remove(node.get()->left, id);
        return std::move(node);
    }

    if (id > node.get()->wall.get()->getId())
    {
        node.get()->right = remove(node.get()->right, id);
        return std::move(node);
    }

    // Left child does not exist
    if (node.get()->left == nullptr)
    {
        return std::move(node.get()->right);
    }

    // Right child does not exist
    if (node.get()->right == nullptr)
    {
        return std::move(node.get()->left);
    }

    // Both children exist
    std::shared_ptr<Wall> wall = findMin(node);

    node.get()->wall = wall;
    node.get()->right = remove(node.get()->right, wall->getId());
    return std::move(node);
}

std::shared_ptr<Wall> WallTree::search(const std::unique_ptr<WallNode> &node, wall_id id) const
{
    if (node == nullptr)
    {
        return nullptr;
    }

    wall_id wallId = node.get()->wall.get()->getId();

    if (id == wallId)
    {
        return node.get()->wall;
    }
    
    return search((id <= wallId) ? node.get()->left : node.get()->right, id);
}

void WallTree::drawWalls(std::unique_ptr<WallNode> &node, const Renderer *renderer,
                         int gridLeft, int gridRight, int gridTop, int gridBottom)
{
    if (node == nullptr)
    {
        return;
    }

    drawWalls(node.get()->left, renderer, gridLeft, gridRight, gridTop, gridBottom);
    node.get()->wall.get()->drawObj(renderer, gridLeft, gridRight, gridTop, gridBottom);
    drawWalls(node.get()->right, renderer, gridLeft, gridRight, gridTop, gridBottom);
}

std::shared_ptr<Wall> WallTree::findMin(std::unique_ptr<WallNode> &node)
{
    if (node == nullptr)
    {
        return nullptr;
    }

    if (node.get()->left == nullptr && node.get()->right == nullptr)
    {
        return node.get()->wall;
    }

    if (node.get()->left)
    {
        return findMin(node.get()->left);
    }

    return findMin(node.get()->right);
}