#include <memory>
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

void WallTree::drawWalls(const Renderer *const renderer, int gridLeft, int gridRight, int gridTop, int gridBottom) const
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

    insert((newWall->getId() <= node->wall->getId()) ? node->left : node->right, newWall);
}

std::unique_ptr<WallTree::WallNode> WallTree::remove(std::unique_ptr<WallNode> &node, wall_id id)
{
    if (node == nullptr)
    {
        return nullptr;
    }

    if (id < node->wall->getId())
    {
        node->left = remove(node->left, id);
        return std::move(node);
    }

    if (id > node->wall->getId())
    {
        node->right = remove(node->right, id);
        return std::move(node);
    }

    // Left child does not exist
    if (node->left == nullptr)
    {
        return std::move(node->right);
    }

    // Right child does not exist
    if (node->right == nullptr)
    {
        return std::move(node->left);
    }

    // Both children exist
    std::shared_ptr<Wall> wall = findMin(node->right);

    node->wall = wall;
    node->right = remove(node->right, wall->getId());
    return std::move(node);
}

std::shared_ptr<Wall> WallTree::search(const std::unique_ptr<WallNode> &node, wall_id id) const
{
    if (node == nullptr)
    {
        return nullptr;
    }

    wall_id wallId = node->wall->getId();

    if (id == wallId)
    {
        return node->wall;
    }
    
    return search((id <= wallId) ? node->left : node->right, id);
}

void WallTree::drawWalls(const std::unique_ptr<WallNode> &node, const Renderer *const renderer,
                         int gridLeft, int gridRight, int gridTop, int gridBottom) const
{
    if (node == nullptr)
    {
        return;
    }

    drawWalls(node->left, renderer, gridLeft, gridRight, gridTop, gridBottom);
    node->wall->drawObj(renderer, gridLeft, gridRight, gridTop, gridBottom);
    drawWalls(node->right, renderer, gridLeft, gridRight, gridTop, gridBottom);
}

std::shared_ptr<Wall> WallTree::findMin(const std::unique_ptr<WallNode> &node) const
{
    if (node == nullptr)
    {
        return nullptr;
    }

    if (node->left == nullptr && node->right == nullptr)
    {
        return node->wall;
    }

    if (node->left)
    {
        return findMin(node->left);
    }

    return findMin(node->right);
}