#pragma once

#include <memory>
#include "wall.hpp"
#include "renderer.hpp"

// BST for storing walls sorted by their ids.
class WallTree
{
public:
    WallTree() = default;
    ~WallTree() = default;
    void insert(const std::shared_ptr<Wall> &newWall);
    void remove(wall_id id);
    std::shared_ptr<Wall> search(wall_id id) const;
    void drawWalls(const Renderer *renderer, int gridLeft, int gridRight,\
                   int gridTop, int gridBottom);

private:
    struct WallNode
    {
        WallNode(std::shared_ptr<Wall> newWall)
            : wall(newWall) {}
        std::shared_ptr<Wall> wall;
        std::unique_ptr<WallNode> left;
        std::unique_ptr<WallNode> right;
    };
    
    std::unique_ptr<WallNode> root;
    void insert(std::unique_ptr<WallNode> &node, const std::shared_ptr<Wall> &newWall);
    std::unique_ptr<WallNode> remove(std::unique_ptr<WallNode> &node, wall_id id);
    std::shared_ptr<Wall> search(const std::unique_ptr<WallNode> &node, wall_id id) const;
    void drawWalls(std::unique_ptr<WallNode> &node, const Renderer *renderer,\
                   int gridLeft, int gridRight, int gridTop, int gridBottom);
    std::shared_ptr<Wall> findMin(std::unique_ptr<WallNode> &node);
};

