#pragma once

#include <memory>
#include <vector>
#include "renderer.hpp"
#include "modemenu.hpp"
#include "vertex.hpp"
#include "wall.hpp"
#include "vertextree.hpp"
#include "vertexgraph.hpp"
#include "walltree.hpp"

#define DEFAULT_GRID_WIDTH 200
#define DEFAULT_GRID_HEIGHT 200
#define DEFAULT_GRID_LEFT 0
#define DEFAULT_GRID_RIGHT DEFAULT_GRID_WIDTH
#define DEFAULT_GRID_TOP 0
#define DEFAULT_GRID_BOTTOM DEFAULT_GRID_HEIGHT
#define VERTEX_SNAP_DIST 3

class Grid
{
public:
    Grid() = default;
    ~Grid() = default;
    void init(int xpos, int ypos, int width, int height, Renderer *gridRenderer);
    void drawGrid() const;
    void drawGridItems();
    void selectMode();
    void cancelSelectMode();
    void wallMode();
    void cancelWallMode();
    void windowCoordToGridCoord(int &x, int &y);
    void gridCoordToWindowCoord(int &x, int &y);

private:
    int gridLeft;
    int gridRight;
    int gridTop;
    int gridBottom;
    Renderer *renderer;
    VertexTree vertexTree;
    WallTree wallTree;
    VertexGraph graph;
    std::shared_ptr<Wall> newWall;
    std::shared_ptr<GridObject> selectedObj;

    int selectedVertexOriginalCoords[2];
    bool selectModeLeftButtonClicked = false;
    bool selectedVertexRemoved = false;
    bool wallModeLeftButtonClicked = false;
    void drawWalls();
    void drawVertices();
    bool inGrid(int x, int y) const;
    void forceCoordsInGrid(int &x, int &y) const;
    bool sameCoords(const int coords1[2], const int coords2[2]) const;
    void updateSelectedObj();
    bool snapToNeighbourVertex(std::shared_ptr<Vertex> &vertex);
    void updateSelectedObjOnSnap(std::shared_ptr<Vertex> &selectedObjVertexCasted);
};