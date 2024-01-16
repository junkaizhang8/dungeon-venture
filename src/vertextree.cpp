#include <iostream>
#include <memory>
#include "vertex.hpp"
#include "vertextree.hpp"
#include "renderer.hpp"

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

std::shared_ptr<Vertex> VertexTree::proximitySearch(const int coords[2], double dist, int vertexId)
{
    if (dist < 0)
    {
        return nullptr;
    }

    return proximitySearch(root, coords, dist * dist, vertexId, 0);
}

void VertexTree::drawVertices(const Renderer *renderer, int gridLeft, int gridRight,
                              int gridTop, int gridBottom)
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

    std::shared_ptr<Vertex> vertex = node.get()->vertex;

    // If there already exists a vertex at the same coordinate, do not insert newVertex
    if (newVertex.get()->getX() == vertex.get()->getX() && newVertex.get()->getY() == vertex.get()->getY())
    {
        return;
    }

    // If depth is even, check x-coordinate. If depth is odd, check y-coordinate.
    if (!(depth % 2))
    {
        if (newVertex.get()->getX() <= vertex.get()->getX())
        {
            insert(node.get()->left, newVertex, depth + 1);
        }
        else
        {
            insert(node.get()->right, newVertex, depth + 1);
        }
    }
    else
    {
        if (newVertex.get()->getY() <= vertex.get()->getY())
        {
            insert(node.get()->left, newVertex, depth + 1);
        }
        else
        {
            insert(node.get()->right, newVertex, depth + 1);
        }
    }
}

std::unique_ptr<VertexTree::VertexNode> VertexTree::remove(std::unique_ptr<VertexNode> &node,\
                                                           int id, const int coords[2], unsigned depth)
{
    if (node == nullptr)
    {
        return nullptr;
    }

    int dimension = depth % 2;

    std::shared_ptr<Vertex> &vertex = node.get()->vertex;

    if (vertex.get()->getId() == id)
    {
        if (node.get()->right != nullptr)
        {
            std::shared_ptr<Vertex> min = findMin(node.get()->right, dimension, depth + 1);

            node.get()->vertex = min;

            int minCoords[2];
            min->getCoords(minCoords);

            std::unique_ptr<VertexNode> removedNode = remove(node.get()->right, min->getId(), minCoords, depth + 1);
            node.get()->right = std::move(removedNode);
        }
        else if (node.get()->left != nullptr)
        {
            std::shared_ptr<Vertex> min = findMin(node.get()->left, dimension, depth + 1);

            node.get()->vertex = min;

            int minCoords[2];
            min->getCoords(minCoords);

            std::unique_ptr<VertexNode> removedNode = remove(node.get()->left, min->getId(), minCoords, depth + 1);
            node.get()->right = std::move(removedNode);
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
        if (coords[0] <= vertex.get()->getX())
        {
            node.get()->left = remove(node.get()->left, id, coords, depth + 1);
        }
        else
        {
            node.get()->right = remove(node.get()->right, id, coords, depth + 1);
        }
    }
    else
    {
        if (coords[1] <= vertex.get()->getY())
        {
            node.get()->left = remove(node.get()->left, id, coords, depth + 1);
        }
        else
        {
            node.get()->right = remove(node.get()->right, id, coords, depth + 1);
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

    std::shared_ptr<Vertex> vertex = node.get()->vertex;

    if (coords[0] == vertex.get()->getX() && coords[1] == vertex.get()->getY())
    {
        return vertex;
    }

    if (!(depth % 2))
    {
        if (coords[0] <= vertex.get()->getX())
        {
            return search(node.get()->left, coords, depth + 1);
        }
        else
        {
            return search(node.get()->right, coords, depth + 1);
        }
    }
    else
    {
        if (coords[1] <= vertex.get()->getY())
        {
            return search(node.get()->left, coords, depth + 1);
        }
        else
        {
            return search(node.get()->right, coords, depth + 1);
        }
    }
}

std::shared_ptr<Vertex> VertexTree::proximitySearch(const std::unique_ptr<VertexNode> &node, const int coords[2],
                                                    double distSquared, vertex_id vertexId, unsigned depth) const
{
    if (node == nullptr)
    {
        return nullptr;
    }

    std::shared_ptr<Vertex> vertex = node.get()->vertex;

    int x = vertex.get()->getX();
    int y = vertex.get()->getY();
    int dx = x - coords[0];
    int dy = y - coords[1];

    if ((dx * dx) + (dy * dy) <= distSquared && node->vertex->getId() != vertexId)
    {
        return vertex;
    }

    if (!(depth % 2))
    {
        if (coords[0] <= x)
        {
            return proximitySearch(node.get()->left, coords, distSquared, vertexId, depth + 1);
        }
        else
        {
            return proximitySearch(node.get()->right, coords, distSquared, vertexId, depth + 1);
        }
    }
    else
    {
        if (coords[1] <= y)
        {
            return proximitySearch(node.get()->left, coords, distSquared, vertexId, depth + 1);
        }
        else
        {
            return proximitySearch(node.get()->right, coords, distSquared, vertexId, depth + 1);
        }
    }
}

void VertexTree::drawVertices(std::unique_ptr<VertexNode> &node, const Renderer *renderer,
                              int gridLeft, int gridRight, int gridTop, int gridBottom) const
{
    if (node == nullptr)
    {
        return;
    }

    drawVertices(node.get()->left, renderer, gridLeft, gridRight, gridTop, gridBottom);
    node.get()->vertex.get()->drawObj(renderer, gridLeft, gridRight, gridTop, gridBottom);
    drawVertices(node.get()->right, renderer, gridLeft, gridRight, gridTop, gridBottom);
}

std::shared_ptr<Vertex> VertexTree::findMin(std::unique_ptr<VertexNode> &node, int dimension, unsigned depth)
{
    if (node == nullptr)
    {
        return nullptr;
    }

    int currentDimension = depth % 2;

    if (currentDimension == dimension)
    {
        // There are no vertices with a smaller value at the corresponding dimension
        if (node.get()->left == nullptr)
        {
            return node.get()->vertex;
        }

        return findMin(node.get()->left, dimension, depth + 1);
    }

    // Minimum can be any of the nodes in the subtree
    return determineMin(node.get()->vertex,
                        findMin(node.get()->left, dimension, depth + 1),
                        findMin(node.get()->right, dimension, depth + 1), dimension);
}

// Determines the minimum vertex at dimension out of v1, v2, and v3
std::shared_ptr<Vertex> VertexTree::determineMin(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2, std::shared_ptr<Vertex> v3, int dimension)
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