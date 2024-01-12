#include <memory>
#include <cmath>
#include "renderer.hpp"
#include "cursor.hpp"
#include "wall.hpp"
#include "vertextree.hpp"
#include "grid.hpp"

void Grid::init(int left, int right, int top, int bottom, std::shared_ptr<Renderer> gridRenderer)
{
    gridLeft = left;
    gridRight = right;
    gridTop = top;
    gridBottom = bottom;
    renderer = gridRenderer;
}

void Grid::drawGrid()
{
    int rowAndColumnNum = 10;
    double gridlineWidth = 0.2;
    double gridlineWidthHalf = gridlineWidth / 2;
    int cellWidth = gridRight / rowAndColumnNum;
    int cellHeight = gridBottom / rowAndColumnNum;

    for (int i = 0; i <= rowAndColumnNum; i++)
    {
        renderer.get()->drawRect(cellWidth * i - gridlineWidthHalf + gridLeft, gridTop, gridlineWidth, gridBottom, WHITE);
        renderer.get()->drawRect(gridLeft, cellHeight * i - gridlineWidthHalf + gridTop, gridRight, gridlineWidth, WHITE);
    }
}

void Grid::drawGridItems()
{
    drawWalls();
    drawVertices();
}

void Grid::selectMode()
{
    int cursorX;
    int cursorY;

    if (!onClickInGrid())
    {
        return;
    }

    Cursor::getScaledCursorPos(cursorX, cursorY);

    if (Cursor::leftButtonDown())
    {
        // Force the cursor position to be within the grid so that dragging the
        // vertices won't go beyond the grid boundaries
        forceCoordsInGrid(cursorX, cursorY);

        windowCoordToGridCoord(cursorX, cursorY);
    }

    int coords[2] = {cursorX, cursorY};
    std::shared_ptr<Vertex> selectedVertex = vertexTree.proximitySearch(coords, GRID_VERTEX_RADIUS);

    
}

void cancelSelectMode() {}

void Grid::wallMode()
{
    int cursorX;
    int cursorY;

    if (!onClickInGrid())
    {
        return;
    }

    Cursor::getScaledCursorPos(cursorX, cursorY);

    if (Cursor::leftButtonDown())
    {
        // Force the cursor position to be within the grid so that the wall
        // won't go beyond the grid boundaries
        forceCoordsInGrid(cursorX, cursorY);

        windowCoordToGridCoord(cursorX, cursorY);

        std::shared_ptr<Vertex> neighbourVertex;
        int coords[2] = {cursorX, cursorY};

        if (newWall.getStartVertex() == nullptr)
        {
            if ((neighbourVertex = vertexTree.proximitySearch(coords, VERTEX_SNAP_DIST)))
            {
                newWall.setStartVertex(neighbourVertex);
            } else
            {
                newWall.setStartVertex(std::make_shared<Vertex>(cursorX, cursorY));
            }
        }
        
        if (newWall.getEndVertex() == nullptr)
        {
            newWall.setEndVertex(std::make_shared<Vertex>(cursorX, cursorY));
        } else
        {
            newWall.getEndVertex()->setX(cursorX);
            newWall.getEndVertex()->setY(cursorY);
        }

        newWall.getStartVertex().get()->drawObj(renderer, gridLeft, gridRight, gridTop, gridBottom);
        newWall.drawObj(renderer, gridLeft, gridRight, gridTop, gridBottom);
        newWall.getEndVertex().get()->drawObj(renderer, gridLeft, gridRight, gridTop, gridBottom);
    } else
    {
        if (!newWall.getStartVertex() || !newWall.getEndVertex())
        {
            return;
        }

        std::shared_ptr<Vertex> neighbourVertex;
        int coords[2];

        newWall.getEndVertex().get()->getCoords(coords);

        if ((neighbourVertex = vertexTree.proximitySearch(coords, VERTEX_SNAP_DIST)))
        {
            newWall.setEndVertex(neighbourVertex);
        }

        int startCoords[2];
        int endCoords[2];

        newWall.getStartVertex().get()->getCoords(startCoords);
        newWall.getEndVertex().get()->getCoords(endCoords);

        if (!sameCoords(startCoords, endCoords))
        {
            vertexTree.insert(newWall.getStartVertex());
            vertexTree.insert(newWall.getEndVertex());
            wallList.push_back(newWall);
            graph.insertMapping(newWall);
        }

        Wall wall;
        newWall = wall;
    }
}

void Grid::cancelWallMode()
{
    newWall.resetVertices();
}

bool Grid::newWallDrawn()
{
    if (newWallDrawnFlag)
    {
        newWallDrawnFlag = false;
        return true;
    }

    return false;
}

/*
 * Rescale window coordinates given by arguments x and y to grid coordinates.
 * In grid coordinates, instead of having (0, 0) be on the top left of grid,
 * it would be at the center of the grid, analogous to the origin
 * of the Cartesian plane.
*/
void Grid::windowCoordToGridCoord(int &x, int &y)
{
    x -= gridRight / 2;
    y -= gridBottom / 2;
}

// Rescale grid coordinates given by arguments x and y to window coordinates.
void Grid::gridCoordToWindowCoord(int &x, int &y)
{
    x += gridRight / 2;
    y += gridBottom / 2;
}

void Grid::drawWalls()
{
    for (Wall wall : wallList)
    {
        wall.drawObj(renderer, gridLeft, gridRight, gridTop, gridBottom);
    }
}

void Grid::drawVertices()
{
    vertexTree.drawVertices(renderer, gridLeft, gridRight, gridTop, gridBottom);
}

// Return if (x, y) is in the grid.
bool Grid::inGrid(int x, int y)
{
    return (x >= gridLeft) && (x <= gridRight) && (y >= gridTop) && (y <= gridBottom);
}

// Return if the clicking position of cursor is in the grid.
bool Grid::onClickInGrid()
{
    int x;
    int y;
    Cursor::getScaledOnLeftClickPos(x, y);

    return inGrid(x, y);
}

// Force x and y to be within the grid boundaries if they exceed the boundaries.
void Grid::forceCoordsInGrid(int &x, int &y)
{
    if (x < gridLeft)
    {
        x = gridLeft;
    }

    if (x > gridRight)
    {
        x = gridRight;
    }

    if (y < gridTop)
    {
        y = gridTop;
    }

    if (y > gridBottom)
    {
        y = gridBottom;
    }
}

bool Grid::sameCoords(const int coords1[2], const int coords2[2])
{
    return (coords1[0] == coords2[0]) && (coords1[1] == coords2[1]);
}