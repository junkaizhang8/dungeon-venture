#include <memory>
#include "vertex.hpp"
#include "renderer.hpp"
#include "vertextree.hpp"

// Return an iterator pointing to the first (in-order) element in the tree.
VertexTreeIterator VertexTree::begin()
{
    if (root == nullptr)
    {
        return end();
    }

    VertexNode *temp = root.get();

    while (temp->left != nullptr)
    {
        temp = temp->left.get();
    }

    return VertexTreeIterator(temp, this);
}

// Return an iterator pointing to the element immediately after the last
// element in the tree.
VertexTreeIterator VertexTree::end()
{
    return VertexTreeIterator(nullptr, this);
}

void VertexTree::insert(const std::shared_ptr<Vertex> &newVertex)
{
    insert(root, nullptr, newVertex, 0);
}

void VertexTree::remove(int id, const int coords[2])
{
    root = remove(root, nullptr, id, coords, 0);
}

std::shared_ptr<Vertex> VertexTree::search(const int coords[2]) const
{
    return search(root, coords, 0);
}

std::shared_ptr<Vertex> VertexTree::proximitySearch(const int coords[2], double dist) const
{
    if (dist < 0)
    {
        return nullptr;
    }

    return proximitySearch(root, coords, dist * dist, 0);
}

void VertexTree::insert(std::unique_ptr<VertexNode> &node, VertexNode *const prevNode,
                        const std::shared_ptr<Vertex> &newVertex, unsigned depth)
{
    if (node == nullptr)
    {
        node = std::make_unique<VertexNode>(newVertex);
        node->parent = prevNode;
        return;
    }

    std::shared_ptr<Vertex> vertex = node->vertex;

    int coords[2];

    newVertex->getCoords(coords);

    // If there already exists a vertex at the same coordinate, do not insert newVertex
    if (coords[0] == vertex->getX() && coords[1] == vertex->getY())
    {
        return;
    }

    if (goLeft(node, coords, depth))
    {
        insert(node->left, node.get(), newVertex, depth + 1);
    }
    else
    {
        insert(node->right, node.get(), newVertex, depth + 1);
    }
}

std::unique_ptr<VertexTree::VertexNode> VertexTree::remove(std::unique_ptr<VertexNode> &node,
                                                           VertexNode *const prevNode, int id,
                                                           const int coords[2], unsigned depth)
{
    if (node == nullptr)
    {
        return nullptr;
    }

    int dimension = depth % 2;

    std::shared_ptr<Vertex> &vertex = node->vertex;

    if (vertex->getId() == id)
    {
        if (node->right)
        {
            std::shared_ptr<Vertex> min = findMin(node->right, dimension, depth + 1);

            node->vertex = min;

            int minCoords[2];
            min->getCoords(minCoords);

            std::unique_ptr<VertexNode> removedNode = remove(node->right, node.get(), min->getId(), minCoords, depth + 1);
            node->right = std::move(removedNode);

            if (node->right)
            {
                node->right->parent = node.get();
            }
        }
        else if (node->left)
        {
            std::shared_ptr<Vertex> min = findMin(node->left, dimension, depth + 1);

            node->vertex = min;

            int minCoords[2];
            min->getCoords(minCoords);

            std::unique_ptr<VertexNode> removedNode = remove(node->left, node.get(), min->getId(), minCoords, depth + 1);
            node->right = std::move(removedNode);

            if (node->left)
            {
                node->left->parent = node.get();
            }
        }
        else
        {
            node.reset();
        }

        return std::move(node);
    }

    // If depth is even, check x-coordinate. If depth is odd, check y-coordinate.
    if (!dimension)
    {
        if (coords[0] <= vertex->getX())
        {
            node->left = remove(node->left, node.get(), id, coords, depth + 1);
        }
        else
        {
            node->right = remove(node->right, node.get(), id, coords, depth + 1);
        }
    }
    else
    {
        if (coords[1] <= vertex->getY())
        {
            node->left = remove(node->left, node.get(), id, coords, depth + 1);
        }
        else
        {
            node->right = remove(node->right, node.get(), id, coords, depth + 1);
        }
    }

    return std::move(node);
}

std::shared_ptr<Vertex> VertexTree::search(const std::unique_ptr<VertexNode> &node,
                                           const int coords[2], unsigned depth) const
{
    if (node == nullptr)
    {
        return nullptr;
    }

    std::shared_ptr<Vertex> vertex = node->vertex;

    if (coords[0] == vertex->getX() && coords[1] == vertex->getY())
    {
        return vertex;
    }

    if (!(depth % 2))
    {
        if (coords[0] <= vertex->getX())
        {
            return search(node->left, coords, depth + 1);
        }
        else
        {
            return search(node->right, coords, depth + 1);
        }
    }
    else
    {
        if (coords[1] <= vertex->getY())
        {
            return search(node->left, coords, depth + 1);
        }
        else
        {
            return search(node->right, coords, depth + 1);
        }
    }
}

std::shared_ptr<Vertex> VertexTree::proximitySearch(const std::unique_ptr<VertexNode> &node, const int coords[2],
                                                    double distSquared, unsigned depth) const
{
    if (node == nullptr)
    {
        return nullptr;
    }

    std::shared_ptr<Vertex> vertex = node->vertex;

    int x = vertex->getX();
    int y = vertex->getY();
    int dx = x - coords[0];
    int dy = y - coords[1];

    if ((dx * dx) + (dy * dy) <= distSquared)
    {
        return vertex;
    }

    if (!(depth % 2))
    {
        if (coords[0] <= x)
        {
            return proximitySearch(node->left, coords, distSquared, depth + 1);
        }
        else
        {
            return proximitySearch(node->right, coords, distSquared, depth + 1);
        }
    }
    else
    {
        if (coords[1] <= y)
        {
            return proximitySearch(node->left, coords, distSquared, depth + 1);
        }
        else
        {
            return proximitySearch(node->right, coords, distSquared, depth + 1);
        }
    }
}

std::shared_ptr<Vertex> VertexTree::findMin(const std::unique_ptr<VertexNode> &node, int dimension, unsigned depth)
{
    if (node == nullptr)
    {
        return nullptr;
    }

    int currentDimension = depth % 2;

    if (currentDimension == dimension)
    {
        // There are no vertices with a smaller value at the corresponding dimension
        if (node->left == nullptr)
        {
            return node->vertex;
        }

        return findMin(node->left, dimension, depth + 1);
    }

    // Minimum can be any of the nodes in the subtree
    return determineMin(node->vertex,
                        findMin(node->left, dimension, depth + 1),
                        findMin(node->right, dimension, depth + 1), dimension);
}

// Determines the minimum vertex at dimension out of v1, v2, and v3
std::shared_ptr<Vertex> VertexTree::determineMin(const std::shared_ptr<Vertex> &v1,
                                                 const std::shared_ptr<Vertex> &v2,
                                                 const std::shared_ptr<Vertex> &v3,
                                                 int dimension)
{
    std::shared_ptr<Vertex> minVertex = v1;

    if (!dimension)
    {
        if (v2 && v2->getX() < minVertex->getX())
        {
            minVertex = v2;
        }

        if (v3 && v3->getX() < minVertex->getX())
        {
            minVertex = v3;
        }
    }
    else
    {
        if (v2 && v2->getY() < minVertex->getY())
        {
            minVertex = v2;
        }

        if (v3 && v3->getY() < minVertex->getY())
        {
            minVertex = v3;
        }
    }

    return minVertex;
}

/*
 * Return true if coords should be located at the left of node.
 * Return false otherwise.
 *
 * Precondition: node must not be a nullptr.
 */
bool VertexTree::goLeft(const std::unique_ptr<VertexNode> &node, const int coords[2], unsigned depth)
{
    int vertexCoords[2];

    node->vertex->getCoords(vertexCoords);

    if (!(depth % 2))
    {
        if (coords[0] <= vertexCoords[0])
        {
            return true;
        }

        return false;
    }
    else
    {
        if (coords[1] <= vertexCoords[1])
        {
            return true;
        }

        return false;
    }
}

// Pre-increment operator
VertexTreeIterator &VertexTreeIterator::operator++()
{
    const VertexTree::VertexNode *temp;

    // Set node to be the smallest element if node is empty
    if (node == nullptr)
    {
        node = tree->root.get();

        while (node->left != nullptr)
        {
            node = node->left.get();
        }
    }
    else
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
VertexTreeIterator VertexTreeIterator::operator++(int)
{
    VertexTreeIterator it = *this;
    const VertexTree::VertexNode *temp;

    // Set node to be the smallest element if node is empty
    if (node == nullptr)
    {
        node = tree->root.get();

        while (node->left != nullptr)
        {
            node = node->left.get();
        }
    }
    else
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