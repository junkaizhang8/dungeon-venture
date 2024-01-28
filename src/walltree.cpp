#include <memory>
#include "walltree.hpp"

// Return an iterator pointing to the first (in-order) element in the tree.
WallTreeIterator WallTree::begin()
{
    if (root == nullptr)
    {
        return end();
    }

    WallNode *temp = root.get();

    while (temp->left != nullptr)
    {
        temp = temp->left.get();
    }

    return WallTreeIterator(temp, this);
}

// Return an iterator pointing to the element immediately after the last
// element in the tree.
WallTreeIterator WallTree::end()
{
    return WallTreeIterator(nullptr, this);
}

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

// Pre-increment operator
WallTreeIterator &WallTreeIterator::operator++()
{
    const WallTree::WallNode *temp;

    // Set node to be the smallest element if node is empty
    if (node == nullptr)
    {
        node = tree->root.get();

        while (node->left != nullptr)
        {
            node = node->left.get();
        }
    } else
    {
        // If there is a right subtree, set node to be the smallest node in subtree
        if (node->right != nullptr)
        {
            node = node->right.get();

            while (node->left != nullptr)
            {
                node = node->left.get();
            }
        }
        // If there is no right subtree, then either:
        // 1. set node to be its parent node if node is a left child of parent
        // 2. set node to be the parent node of the root of the right subtree
        else
        {
            temp = node->parent;
            
            while (temp != nullptr && temp->right && node == temp->right.get())
            {
                node = temp;
                temp = temp->parent;
            }

            node = temp;
        }
    }

    return *this;
}

// Post-increment operator
WallTreeIterator WallTreeIterator::operator++(int)
{
    WallTreeIterator it = *this;
    const WallTree::WallNode *temp;

    // Set node to be the smallest element if node is empty
    if (node == nullptr)
    {
        node = tree->root.get();

        while (node->left != nullptr)
        {
            node = node->left.get();
        }
    } else
    {
        // If there is a right subtree, set node to be the smallest node in subtree
        if (node->right != nullptr)
        {
            node = node->right.get();

            while (node->left != nullptr)
            {
                node = node->left.get();
            }
        }
        // If there is no right subtree, then either:
        // 1. set node to be its parent node if node is a left child of parent
        // 2. set node to be the parent node of the root of the right subtree
        else
        {
            temp = node->parent;
            
            while (temp != nullptr && temp->right && node == temp->right.get())
            {
                node = temp;
                temp = temp->parent;
            }

            node = temp;
        }
    }

    return it;
}