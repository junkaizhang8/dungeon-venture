#include <memory>
#include "vertex.hpp"
#include "vertextree.hpp"

void VertexTree::insert(const std::shared_ptr<Vertex> &newVertex)
{
    insert(root, newVertex, 0);
}

std::shared_ptr<Vertex> VertexTree::search(const int coords[2])
{
    return search(root, coords, 0);
}

std::shared_ptr<Vertex> VertexTree::proximitySearch(const int coords[2], double dist)
{
    if (dist < 0)
    {
        return nullptr;
    }

    return proximitySearch(root, coords, dist * dist, 0);
}


void VertexTree::insert(std::shared_ptr<VertexNode> &node, const std::shared_ptr<Vertex> &newVertex, unsigned depth)
{
    if (node == nullptr)
    {
        node = std::make_shared<VertexNode>(newVertex);
        return;
    }

    std::shared_ptr<Vertex> vertex = node.get()->vertex;

    // If depth is even, check x-coordinate. If depth is odd, check y-coordinate.
    if (!(depth % 2))
    {
        if (newVertex.get()->getX() <= vertex.get()->getX())
        {
            insert(node.get()->left, newVertex, depth + 1);
        } else
        {
            insert(node.get()->right, newVertex, depth + 1);
        }
    } else
    {
        if (newVertex.get()->getY() <= vertex.get()->getY())
        {
            insert(node.get()->left, newVertex, depth + 1);
        } else
        {
            insert(node.get()->right, newVertex, depth + 1);
        }
    }
}

std::shared_ptr<Vertex> VertexTree::search(const std::shared_ptr<VertexNode> &node, const int coords[2], unsigned depth)
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
        } else
        {
            return search(node.get()->right, coords, depth + 1);
        }
    } else
    {
        if (coords[1] <= vertex.get()->getY())
        {
            return search(node.get()->left, coords, depth + 1);
        } else
        {
            return search(node.get()->right, coords, depth + 1);
        }
    }
}

std::shared_ptr<Vertex> VertexTree::proximitySearch(const std::shared_ptr<VertexNode> &node, const int coords[2],\
                                                    double distSquared, unsigned depth)
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

    if ((dx * dx) + (dy * dy) <= distSquared)
    {
        return vertex;
    }

    if (!(depth % 2))
    {
        if (coords[0] <= x)
        {
            return proximitySearch(node.get()->left, coords, distSquared, depth + 1);
        } else
        {
            return proximitySearch(node.get()->right, coords, distSquared, depth + 1);
        }
    } else
    {
        if (coords[1] <= y)
        {
            return proximitySearch(node.get()->left, coords, distSquared, depth + 1);
        } else
        {
            return proximitySearch(node.get()->right, coords, distSquared, depth + 1);
        }
    }
}