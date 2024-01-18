#include <memory>
#include "vertex.hpp"
#include "renderer.hpp"
#include "vertextree.hpp"

void VertexTree::insert(const std::shared_ptr<Vertex> &newVertex)
{
    insert(root, newVertex, 0);
}

void VertexTree::remove(int id, const int coords[2])
{
    root = remove(root, id, coords, 0);
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

void VertexTree::drawVertices(const Renderer *const renderer, int gridLeft, int gridRight,
                              int gridTop, int gridBottom) const
{
    drawVertices(root, renderer, gridLeft, gridRight, gridTop, gridBottom);
}

void VertexTree::insert(std::unique_ptr<VertexNode> &node, const std::shared_ptr<Vertex> &newVertex,
                        unsigned depth)
{
    if (node == nullptr)
    {
        node = std::make_unique<VertexNode>(newVertex);
        return;
    }

    std::shared_ptr<Vertex> vertex = node->vertex;

    // If there already exists a vertex at the same coordinate, do not insert newVertex
    if (newVertex->getX() == vertex->getX() && newVertex->getY() == vertex->getY())
    {
        return;
    }

    // If depth is even, check x-coordinate. If depth is odd, check y-coordinate.
    if (!(depth % 2))
    {
        if (newVertex->getX() <= vertex->getX())
        {
            insert(node->left, newVertex, depth + 1);
        }
        else
        {
            insert(node->right, newVertex, depth + 1);
        }
    }
    else
    {
        if (newVertex->getY() <= vertex->getY())
        {
            insert(node->left, newVertex, depth + 1);
        }
        else
        {
            insert(node->right, newVertex, depth + 1);
        }
    }
}

std::unique_ptr<VertexTree::VertexNode> VertexTree::remove(std::unique_ptr<VertexNode> &node,
                                                           int id, const int coords[2], unsigned depth)
{
    if (node == nullptr)
    {
        return nullptr;
    }

    int dimension = depth % 2;

    std::shared_ptr<Vertex> &vertex = node->vertex;

    if (vertex->getId() == id)
    {
        if (node->right != nullptr)
        {
            std::shared_ptr<Vertex> min = findMin(node->right, dimension, depth + 1);

            node->vertex = min;

            int minCoords[2];
            min->getCoords(minCoords);

            std::unique_ptr<VertexNode> removedNode = remove(node->right, min->getId(), minCoords, depth + 1);
            node->right = std::move(removedNode);
        }
        else if (node->left != nullptr)
        {
            std::shared_ptr<Vertex> min = findMin(node->left, dimension, depth + 1);

            node->vertex = min;

            int minCoords[2];
            min->getCoords(minCoords);

            std::unique_ptr<VertexNode> removedNode = remove(node->left, min->getId(), minCoords, depth + 1);
            node->right = std::move(removedNode);
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
            node->left = remove(node->left, id, coords, depth + 1);
        }
        else
        {
            node->right = remove(node->right, id, coords, depth + 1);
        }
    }
    else
    {
        if (coords[1] <= vertex->getY())
        {
            node->left = remove(node->left, id, coords, depth + 1);
        }
        else
        {
            node->right = remove(node->right, id, coords, depth + 1);
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

void VertexTree::drawVertices(const std::unique_ptr<VertexNode> &node, const Renderer *const renderer,
                              int gridLeft, int gridRight, int gridTop, int gridBottom) const
{
    if (node == nullptr)
    {
        return;
    }

    drawVertices(node->left, renderer, gridLeft, gridRight, gridTop, gridBottom);
    node->vertex->drawObj(renderer, gridLeft, gridRight, gridTop, gridBottom);
    drawVertices(node->right, renderer, gridLeft, gridRight, gridTop, gridBottom);
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