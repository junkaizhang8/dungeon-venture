#include <memory>
#include <cmath>
#include "renderer.hpp"
#include "cursor.hpp"
#include "vertex.hpp"
#include "wall.hpp"
#include "vertextree.hpp"
#include "vertexgraph.hpp"
#include "walltree.hpp"
#include "grid.hpp"

/*
 * Initialize the grid specified by its boundary positions in the window and
 * the renderer assigned to the window
 */
void Grid::init(int left, int right, int top, int bottom, Renderer *gridRenderer)
{
    gridLeft = left;
    gridRight = right;
    gridTop = top;
    gridBottom = bottom;
    renderer = gridRenderer;
    newWall = std::make_shared<Wall>();
}

void Grid::drawGrid() const
{
    int rowAndColumnNum = 10;
    double gridlineWidth = 0.2;
    double gridlineWidthHalf = gridlineWidth / 2;
    int cellWidth = gridRight / rowAndColumnNum;
    int cellHeight = gridBottom / rowAndColumnNum;

    for (int i = 0; i <= rowAndColumnNum; i++)
    {
        renderer->drawRect(cellWidth * i - gridlineWidthHalf + gridLeft, gridTop, gridlineWidth, gridBottom, WHITE);
        renderer->drawRect(gridLeft, cellHeight * i - gridlineWidthHalf + gridTop, gridRight, gridlineWidth, WHITE);
    }
}

void Grid::drawGridItems()
{
    drawWalls();
    drawVertices();

    if (selectedObj)
    {
        selectedObj->drawObjSelected(renderer, gridLeft, gridRight, gridTop, gridBottom);
    }
}

void Grid::selectMode()
{
    int onLeftClickCursorX;
    int onLeftClickCursorY;
    int onRightClickCursorX;
    int onRightClickCursorY;

    // Check for if right mouse button is in down position and in the grid
    // If true, unselect the selected object
    if (Cursor::rightButtonDown())
    {
        Cursor::getScaledOnRightClickPos(onRightClickCursorX, onRightClickCursorY);

        if (inGrid(onRightClickCursorX, onRightClickCursorY))
        {
            selectedObj = nullptr;
            return;
        }
    }

    Cursor::getScaledOnLeftClickPos(onLeftClickCursorX, onLeftClickCursorY);

    if (!inGrid(onLeftClickCursorX, onLeftClickCursorY))
    {
        return;
    }

    // Check for if left mouse button is in down position and in the grid
    if (Cursor::leftButtonDown())
    {
        if (wallModeLeftButtonClicked)
        {
            return;
        }

        updateSelectedObj();

        selectModeLeftButtonClicked = true;
    }
    else
    {
        wallModeLeftButtonClicked = false;

        if (!selectModeLeftButtonClicked)
        {
            return;
        }

        selectModeLeftButtonClicked = false;

        std::shared_ptr<Vertex> selectedObjVertexCasted = std::dynamic_pointer_cast<Vertex>(selectedObj);
        if (selectedObjVertexCasted)
        {
            // If vertex was snapped to neighbour vertex
            if (snapToNeighbourVertex(selectedObjVertexCasted))
            {
                updateSelectedObjOnSnap(selectedObjVertexCasted);
            }
            else
            {
                vertexTree.insert(selectedObjVertexCasted);
            }

            selectedVertexRemoved = false;
        }
    }
}

void Grid::cancelSelectMode()
{
    if (selectModeLeftButtonClicked)
    {
        selectModeLeftButtonClicked = false;

        if (selectedObj == nullptr)
        {
            return;
        }

        std::shared_ptr<Vertex> selectedObjVertexCasted = std::dynamic_pointer_cast<Vertex>(selectedObj);

        if (selectedObjVertexCasted)
        {
            if (snapToNeighbourVertex(selectedObjVertexCasted))
            {
                updateSelectedObjOnSnap(selectedObjVertexCasted);
            }
            else
            {
                vertexTree.insert(selectedObjVertexCasted);
            }

            selectedVertexRemoved = false;
        }
    }

    selectedObj = nullptr;
}

void Grid::wallMode()
{
    int cursorX;
    int cursorY;
    int onLeftClickCursorX;
    int onLeftClickCursorY;

    Cursor::getScaledOnLeftClickPos(onLeftClickCursorX, onLeftClickCursorY);

    if (!inGrid(onLeftClickCursorX, onLeftClickCursorY))
    {
        return;
    }

    Cursor::getScaledCursorPos(cursorX, cursorY);

    if (Cursor::leftButtonDown())
    {
        if (selectModeLeftButtonClicked)
        {
            return;
        }

        wallModeLeftButtonClicked = true;

        // Force the cursor position to be within the grid so that the wall
        // won't go beyond the grid boundaries
        forceCoordsInGrid(cursorX, cursorY);

        windowCoordToGridCoord(cursorX, cursorY);

        std::shared_ptr<Vertex> neighbourVertex;
        int coords[2] = {cursorX, cursorY};

        if (newWall.get()->getStartVertex() == nullptr)
        {
            if ((neighbourVertex = vertexTree.proximitySearch(coords, VERTEX_SNAP_DIST)))
            {
                newWall.get()->setStartVertex(neighbourVertex);
            }
            else
            {
                newWall.get()->setStartVertex(std::make_shared<Vertex>(cursorX, cursorY));
            }
        }

        if (newWall.get()->getEndVertex() == nullptr)
        {
            newWall.get()->setEndVertex(std::make_shared<Vertex>(cursorX, cursorY));
        }
        else
        {
            newWall.get()->getEndVertex()->setX(cursorX);
            newWall.get()->getEndVertex()->setY(cursorY);
        }

        newWall.get()->getStartVertex().get()->drawObj(renderer, gridLeft, gridRight, gridTop, gridBottom);
        newWall.get()->drawObj(renderer, gridLeft, gridRight, gridTop, gridBottom);
        newWall.get()->getEndVertex().get()->drawObj(renderer, gridLeft, gridRight, gridTop, gridBottom);
    }
    else
    {
        selectModeLeftButtonClicked = false;

        if (!wallModeLeftButtonClicked)
        {
            return;
        }

        wallModeLeftButtonClicked = false;

        if (!newWall.get()->getStartVertex() || !newWall.get()->getEndVertex())
        {
            return;
        }

        std::shared_ptr<Vertex> neighbourVertex;
        int coords[2];

        newWall.get()->getEndVertex().get()->getCoords(coords);

        if ((neighbourVertex = vertexTree.proximitySearch(coords, VERTEX_SNAP_DIST)))
        {
            newWall.get()->setEndVertex(neighbourVertex);
        }

        int startCoords[2];
        int endCoords[2];

        newWall.get()->getStartVertex().get()->getCoords(startCoords);
        newWall.get()->getEndVertex().get()->getCoords(endCoords);

        // If startCoords and endCoords are not equal and there does not already exist
        // a wall between the start vertex and end vertex, save the newWall data.
        // Else, reset newWall.
        if (!sameCoords(startCoords, endCoords) && !graph.contains(newWall->getStartVertex()->getId(),
                                                                   newWall->getEndVertex()->getId()))
        {
            vertexTree.insert(newWall.get()->getStartVertex());
            vertexTree.insert(newWall.get()->getEndVertex());
            wallTree.insert(newWall);
            graph.insertMapping(*newWall);
        }

        newWall.reset(new Wall());
    }
}

void Grid::cancelWallMode()
{
    if (newWall)
    {
        newWall.get()->resetVertices();
    }
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
    wallTree.drawWalls(renderer, gridLeft, gridRight, gridTop, gridBottom);
}

void Grid::drawVertices()
{
    vertexTree.drawVertices(renderer, gridLeft, gridRight, gridTop, gridBottom);
}

// Return if (x, y) is in the grid.
bool Grid::inGrid(int x, int y) const
{
    return (x >= gridLeft) && (x <= gridRight) && (y >= gridTop) && (y <= gridBottom);
}

// Force x and y to be within the grid boundaries if they exceed the boundaries.
void Grid::forceCoordsInGrid(int &x, int &y) const
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

bool Grid::sameCoords(const int coords1[2], const int coords2[2]) const
{
    return (coords1[0] == coords2[0]) && (coords1[1] == coords2[1]);
}

void Grid::updateSelectedObj()
{
    if (selectModeLeftButtonClicked)
    {
        std::shared_ptr<Vertex> selectedObjVertexCasted = std::dynamic_pointer_cast<Vertex>(selectedObj);

        if (selectedObjVertexCasted)
        {
            int selectedObjCoords[2];

            selectedObjVertexCasted->getCoords(selectedObjCoords);

            int cursorCoords[2];
            Cursor::getScaledCursorPos(cursorCoords[0], cursorCoords[1]);
            forceCoordsInGrid(cursorCoords[0], cursorCoords[1]);
            windowCoordToGridCoord(cursorCoords[0], cursorCoords[1]);

            selectedObjVertexCasted.get()->setX(cursorCoords[0]);
            selectedObjVertexCasted.get()->setY(cursorCoords[1]);

            if (cursorCoords[0] != selectedVertexOriginalCoords[0] ||
                cursorCoords[1] != selectedVertexOriginalCoords[1])
            {
                if (!selectedVertexRemoved)
                {
                    vertexTree.remove(selectedObjVertexCasted->getId(), selectedVertexOriginalCoords);
                    selectedVertexRemoved = true;
                }

                selectedVertexOriginalCoords[0] = cursorCoords[0];
                selectedVertexOriginalCoords[1] = cursorCoords[1];
            }
        }
        return;
    }

    int leftClickCoords[2];
    Cursor::getScaledOnLeftClickPos(leftClickCoords[0], leftClickCoords[1]);
    windowCoordToGridCoord(leftClickCoords[0], leftClickCoords[1]);

    std::shared_ptr<Vertex> selectedVertex = vertexTree.proximitySearch(leftClickCoords, GRID_VERTEX_RADIUS);

    if (selectedVertex)
    {
        selectedObj = selectedVertex;

        selectedVertexOriginalCoords[0] = selectedVertex.get()->getX();
        selectedVertexOriginalCoords[1] = selectedVertex.get()->getY();
    }
}

// Return true if vertex was snapped to a neighbouring vertex
// Return false if otherwise
bool Grid::snapToNeighbourVertex(std::shared_ptr<Vertex> &vertex)
{
    if (vertex == nullptr)
    {
        return false;
    }

    int coords[2];
    vertex.get()->getCoords(coords);

    vertex_id vertexId = vertex.get()->getId();

    std::shared_ptr<Vertex> neighbourVertex = vertexTree.proximitySearch(coords, VERTEX_SNAP_DIST, vertexId);

    if (neighbourVertex)
    {
        vertex_id neighbourId = neighbourVertex->getId();

        std::vector<wall_id> modifiedWalls;
        std::vector<wall_id> removedWalls;

        graph.modifyMapping(vertexId, neighbourId, modifiedWalls, removedWalls);

        for (auto wallId : modifiedWalls)
        {
            std::shared_ptr<Wall> wall = wallTree.search(wallId);

            if (wall)
            {
                if (wall.get()->getStartVertex().get()->getId() == vertexId)
                {
                    wall.get()->setStartVertex(neighbourVertex);
                }
                if (wall.get()->getEndVertex().get()->getId() == vertexId)
                {
                    wall.get()->setEndVertex(neighbourVertex);
                }
            }
        }

        for (auto wallId : removedWalls)
        {
            wallTree.remove(wallId);
        }

        vertexTree.remove(vertexId, coords);

        vertex = neighbourVertex;

        return true;
    }

    return false;
}

void Grid::updateSelectedObjOnSnap(std::shared_ptr<Vertex> &selectedObjVertexCasted)
{
    if (selectedObjVertexCasted == nullptr)
    {
        return;
    }

    selectedObj = selectedObjVertexCasted;

    if (graph.contains(selectedObjVertexCasted->getId()))
    {
        selectedObj = selectedObjVertexCasted;
    }
    else
    {
        int coords[2];
        selectedObjVertexCasted->getCoords(coords);

        vertexTree.remove(selectedObjVertexCasted->getId(), coords);

        selectedObj = nullptr;
    }
}