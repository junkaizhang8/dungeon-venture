#pragma once

#include <memory>
#include <vector>
#include "renderer.hpp"
#include "modemenu.hpp"
#include "vertex.hpp"
#include "wall.hpp"
#include "vertextree.hpp"
#include "vertexgraph.hpp"

#define DEFAULT_GRID_WIDTH 200
#define DEFAULT_GRID_HEIGHT 200
#define DEFAULT_GRID_LEFT 0
#define DEFAULT_GRID_RIGHT DEFAULT_GRID_WIDTH
#define DEFAULT_GRID_TOP 0
#define DEFAULT_GRID_BOTTOM DEFAULT_GRID_HEIGHT
#define GRID_WALL_WIDTH 1
#define GRID_VERTEX_RADIUS 1
#define VERTEX_SNAP_DIST 2

class Grid
{
public:
    Grid() = default;
    ~Grid() = default;
    void init(int xpos, int ypos, int width, int height, std::shared_ptr<Renderer> gridRenderer);
    void drawGrid();
    void drawGridItems();
    void drawNewWall();
    void cancelNewWall();
    bool newWallDrawn();

private:
    int gridLeft;
    int gridRight;
    int gridTop;
    int gridBottom;
    bool newWallDrawnFlag = false;
    std::shared_ptr<Renderer> renderer;
    VertexTree tree;
    std::vector<Wall> wallList;
    VertexGraph graph;
    Wall newWall;
    void drawWalls();
    void drawWall(Wall &wall);
    bool inGrid(int x, int y);
    void windowCoordToGridCoord(int &x, int &y);
    void gridCoordToWindowCoord(int &x, int &y);
    bool sameCoords(const int coords1[2], const int coords2[2]);
};