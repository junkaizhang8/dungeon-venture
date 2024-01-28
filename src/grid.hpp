#pragma once

#include <memory>
#include <vector>
#include "renderer.hpp"
#include "modemenu.hpp"
#include "vertex.hpp"
#include "wall.hpp"
#include "mapdata.hpp"

#define DEFAULT_GRID_WIDTH 200
#define DEFAULT_GRID_HEIGHT 200
#define DEFAULT_GRID_LEFT 0
#define DEFAULT_GRID_RIGHT DEFAULT_GRID_WIDTH
#define DEFAULT_GRID_TOP 0
#define DEFAULT_GRID_BOTTOM DEFAULT_GRID_HEIGHT
#define VERTEX_SNAP_DIST 3
#define GRIDLINE_COLOUR WHITE

class Grid
{
public:
    Grid() = default;
    ~Grid() = default;
    void init(int xpos, int ypos, int width, int height, MapData *map, const Renderer *const gridRenderer);
    void drawGrid() const;
    void drawGridItems();
    void selectMode();
    void cancelSelectMode();
    void wallMode();
    void cancelWallMode();
    void windowCoordToGridCoord(int &x, int &y) const;
    void gridCoordToWindowCoord(int &x, int &y) const;

private:
    int gridLeft;
    int gridRight;
    int gridTop;
    int gridBottom;
    const Renderer *renderer;
    MapData *mapData;
    std::shared_ptr<Wall> newWall;
    std::shared_ptr<GridObject> selectedObj;

    int selectedVertexOriginalCoords[2];
    bool selectModeLeftButtonClicked = false;
    bool selectedVertexRemoved = false;
    bool wallModeLeftButtonClicked = false;
    void drawWalls();
    void drawVertices();
    void makeNewWall();
    bool selectedVertexMoved(Vertex *selectedVertex);
    bool inGrid(int x, int y) const;
    void forceCoordsInGrid(int &x, int &y) const;
    bool sameCoords(const int coord1[2], const int coord2[2]) const;
    void updateSelectedObj();
    bool snapToNeighbourVertex(std::shared_ptr<Vertex> &vertex);
    void updateSelectedObjOnSnap(std::shared_ptr<Vertex> &selectedObjVertexCasted);
};