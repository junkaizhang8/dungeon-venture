#include <memory>
#include <cmath>
#include "renderer.hpp"
#include "cursor.hpp"
#include "vertex.hpp"
#include "wall.hpp"
#include "mapdata.hpp"
#include "grid.hpp"

/*
 * Initialize the grid specified by its boundary positions in the window and
 * the renderer assigned to the window.
 */
void Grid::init(int left, int right, int top, int bottom, MapData *data, const Renderer *const gridRenderer)
{
    gridLeft = left;
    gridRight = right;
    gridTop = top;
    gridBottom = bottom;
    mapData = data;
    renderer = gridRenderer;
    newWall = std::make_shared<Wall>();
}

// Draw the gridlines.
void Grid::drawGrid() const
{
    int rowAndColumnNum = 10;
    double gridlineWidth = 0.2;
    double gridlineWidthHalf = gridlineWidth / 2;
    int cellWidth = gridRight / rowAndColumnNum;
    int cellHeight = gridBottom / rowAndColumnNum;

    for (int i = 0; i <= rowAndColumnNum; i++)
    {
        renderer->drawRect(cellWidth * i - gridlineWidthHalf + gridLeft, gridTop,
                           gridlineWidth, gridBottom, GRIDLINE_COLOUR);
        renderer->drawRect(gridLeft, cellHeight * i - gridlineWidthHalf + gridTop,
                           gridRight, gridlineWidth, GRIDLINE_COLOUR);
    }
}

// Draw all grid items.
void Grid::drawGridItems()
{
    drawWalls();
    drawVertices();

    if (selectedObj)
    {
        selectedObj->drawObjSelected(renderer, gridLeft, gridRight, gridTop, gridBottom);
    }
}

/*
 * Enter select mode. During this mode, users can adjust the positions of
 * vertices by dragging them across the grid.
 */
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

    // If clicked position is not in the grid, return
    if (!inGrid(onLeftClickCursorX, onLeftClickCursorY))
    {
        return;
    }

    // Check if left mouse button is in down position and in the grid
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
            // If vertex was snapped to neighbouring vertex
            if (snapToNeighbourVertex(selectedObjVertexCasted))
            {
                updateSelectedObjOnSnap(selectedObjVertexCasted);
            }
            else
            {
                mapData->vertexTree.insert(selectedObjVertexCasted);
            }

            selectedVertexRemoved = false;
        }
    }
}

// Cancel select mode.
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
                mapData->vertexTree.insert(selectedObjVertexCasted);
            }

            selectedVertexRemoved = false;
        }
    }

    selectedObj = nullptr;
}

/*
 * Enter wall mode. During this mode, users can create walls by dragging
 * their cursor around.
 */
void Grid::wallMode()
{
    int cursorX;
    int cursorY;
    int onLeftClickCursorX;
    int onLeftClickCursorY;

    Cursor::getScaledOnLeftClickPos(onLeftClickCursorX, onLeftClickCursorY);

    // If clicked position is not in the grid, return
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

        makeNewWall();
    }
    else
    {
        selectModeLeftButtonClicked = false;

        if (!wallModeLeftButtonClicked)
        {
            return;
        }

        wallModeLeftButtonClicked = false;

        // If newWall is missing a vertex, return
        if (!newWall->getStartVertex() || !newWall->getEndVertex())
        {
            return;
        }

        std::shared_ptr<Vertex> neighbourVertex;
        int coords[2];

        newWall->getEndVertex()->getCoords(coords);

        // If there is a neighbouring vertex to the end vertex,
        // assign it to end vertex of newWall
        if ((neighbourVertex = mapData->vertexTree.proximitySearch(coords, VERTEX_SNAP_DIST)))
        {
            newWall->setEndVertex(neighbourVertex);
        }

        int startCoords[2];
        int endCoords[2];

        newWall->getStartVertex()->getCoords(startCoords);
        newWall->getEndVertex()->getCoords(endCoords);

        // If startCoords and endCoords are not equal and there does not already exist
        // a wall between the start vertex and end vertex, save the newWall data.
        // Else, reset newWall.
        if (!sameCoords(startCoords, endCoords) && !mapData->graph.contains(newWall->getStartVertex()->getId(),
                                                                   newWall->getEndVertex()->getId()))
        {
            mapData->vertexTree.insert(newWall->getStartVertex());
            mapData->vertexTree.insert(newWall->getEndVertex());
            mapData->wallTree.insert(newWall);
            mapData->graph.insertMapping(*newWall);
        }

        newWall.reset(new Wall());
    }
}

// Cancel wall mode.
void Grid::cancelWallMode()
{
    if (newWall)
    {
        newWall->resetVertices();
    }
}

/*
 * Rescale window coordinates given by arguments x and y to grid coordinates.
 * In grid coordinates, instead of having (0, 0) be on the top left of grid,
 * it would be at the center of the grid, analogous to the origin
 * of the Cartesian plane.
 */
void Grid::windowCoordToGridCoord(int &x, int &y) const
{
    x -= gridRight / 2;
    y -= gridBottom / 2;
}

// Rescale grid coordinates given by arguments x and y to window coordinates.
void Grid::gridCoordToWindowCoord(int &x, int &y) const
{
    x += gridRight / 2;
    y += gridBottom / 2;
}

void Grid::drawWalls()
{
    for (auto it = mapData->wallTree.begin(); it != mapData->wallTree.end(); ++it)
    {
        (*it)->drawObj(renderer, gridLeft, gridRight, gridTop, gridBottom);
    }
}

void Grid::drawVertices()
{
    for (auto it = mapData->vertexTree.begin(); it != mapData->vertexTree.end(); ++it)
    {
        (*it)->drawObj(renderer, gridLeft, gridRight, gridTop, gridBottom);
    }
}

// Make a new wall determined by the cursor action.
void Grid::makeNewWall()
{
    int coords[2];

    Cursor::getScaledCursorPos(coords[0], coords[1]);

    // Force the cursor position to be within the grid so that the wall
    // won't go beyond the grid boundaries
    forceCoordsInGrid(coords[0], coords[1]);

    windowCoordToGridCoord(coords[0], coords[1]);

    std::shared_ptr<Vertex> neighbourVertex;

    // If there is no start vertex, either make a new one at coords
    // or assign a neighbouring vertex to it
    if (newWall->getStartVertex() == nullptr)
    {
        if ((neighbourVertex = mapData->vertexTree.proximitySearch(coords, VERTEX_SNAP_DIST)))
        {
            newWall->setStartVertex(neighbourVertex);
        }
        else
        {
            newWall->setStartVertex(std::make_shared<Vertex>(coords));
        }
    }

    // If there is no end vertex, create one at coords
    // Else, update its position using coords
    if (newWall->getEndVertex() == nullptr)
    {
        newWall->setEndVertex(std::make_shared<Vertex>(coords));
    }
    else
    {
        newWall->getEndVertex()->setX(coords[0]);
        newWall->getEndVertex()->setY(coords[1]);
    }

    newWall->getStartVertex()->drawObj(renderer, gridLeft, gridRight,
                                       gridTop, gridBottom);
    newWall->drawObj(renderer, gridLeft, gridRight, gridTop, gridBottom);
    newWall->getEndVertex()->drawObj(renderer, gridLeft, gridRight,
                                     gridTop, gridBottom);
}

// Return true if selectedVertex has been moved.
// Return false otherwise.
bool Grid::selectedVertexMoved(Vertex *const selectedVertex)
{
    if (selectedVertex == nullptr)
    {
        return false;
    }

    int selectedObjCoords[2];
    int cursorCoords[2];

    selectedVertex->getCoords(selectedObjCoords);

    Cursor::getScaledCursorPos(cursorCoords[0], cursorCoords[1]);
    forceCoordsInGrid(cursorCoords[0], cursorCoords[1]);
    windowCoordToGridCoord(cursorCoords[0], cursorCoords[1]);

    selectedVertex->setX(cursorCoords[0]);
    selectedVertex->setY(cursorCoords[1]);

    // If cursor position is different from original position of
    // selectedVertex, update the original position of
    // selectedVertex and return true
    if (cursorCoords[0] != selectedVertexOriginalCoords[0] ||
        cursorCoords[1] != selectedVertexOriginalCoords[1])
    {
        selectedVertexOriginalCoords[0] = cursorCoords[0];
        selectedVertexOriginalCoords[1] = cursorCoords[1];

        return true;
    }

    return false;
}

// Return if (x, y) is in the grid.
// Return false otherwise.
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

// Return true if the coordinates between coord1 and coord2 are the same.
// Return false otherwise.
bool Grid::sameCoords(const int coord1[2], const int coord2[2]) const
{
    return (coord1[0] == coord2[0]) && (coord1[1] == coord2[1]);
}

// Update selectedObj determined by the cursor action.
void Grid::updateSelectedObj()
{
    if (selectModeLeftButtonClicked)
    {
        Vertex *selectedObjVertexCasted = std::dynamic_pointer_cast<Vertex>(selectedObj).get();

        // If selected vertex has been moved, remove selected vertex from
        // mapData->vertexTree to avoid invalid tree traversal behaviour
        if (selectedVertexMoved(selectedObjVertexCasted) && !selectedVertexRemoved)
        {
            mapData->vertexTree.remove(selectedObjVertexCasted->getId(), selectedVertexOriginalCoords);
            selectedVertexRemoved = true;
        }

        return;
    }

    int leftClickCoords[2];
    Cursor::getScaledOnLeftClickPos(leftClickCoords[0], leftClickCoords[1]);
    windowCoordToGridCoord(leftClickCoords[0], leftClickCoords[1]);

    std::shared_ptr<Vertex> selectedVertex = mapData->vertexTree.proximitySearch(leftClickCoords, GRID_VERTEX_RADIUS);

    if (selectedVertex)
    {
        selectedObj = selectedVertex;

        selectedVertexOriginalCoords[0] = selectedVertex->getX();
        selectedVertexOriginalCoords[1] = selectedVertex->getY();
    }
}

// Return true if vertex was modified and snapped to a neighbouring vertex.
// Return false if otherwise.
bool Grid::snapToNeighbourVertex(std::shared_ptr<Vertex> &vertex)
{
    if (vertex == nullptr)
    {
        return false;
    }

    int coords[2];
    vertex->getCoords(coords);

    vertex_id vertexId = vertex->getId();

    std::shared_ptr<Vertex> neighbourVertex = mapData->vertexTree.proximitySearch(coords, VERTEX_SNAP_DIST);

    if (neighbourVertex)
    {
        vertex_id neighbourId = neighbourVertex->getId();

        std::vector<wall_id> modifiedWalls;
        std::vector<wall_id> removedWalls;

        mapData->graph.modifyMapping(vertexId, neighbourId, modifiedWalls, removedWalls);

        // Modify every wall in modifiedWalls by replacing
        // the wall vertex pointing to vertex to be the
        // neighbouring vertex
        for (auto wallId : modifiedWalls)
        {
            std::shared_ptr<Wall> wall = mapData->wallTree.search(wallId);

            if (wall)
            {
                if (wall->getStartVertex()->getId() == vertexId)
                {
                    wall->setStartVertex(neighbourVertex);
                }
                if (wall->getEndVertex()->getId() == vertexId)
                {
                    wall->setEndVertex(neighbourVertex);
                }
            }
        }

        // Remove the walls in removedWalls
        for (auto wallId : removedWalls)
        {
            mapData->wallTree.remove(wallId);
        }

        // Remove the snapped vertex from mapData->vertexTree
        mapData->vertexTree.remove(vertexId, coords);

        vertex = neighbourVertex;

        return true;
    }

    return false;
}

/*
 * Update selectedObj if it has been snapped to a neighbouring vertex.
 * Assign selectedObjVertexCasted to selectedObj if it is a valid
 * vertex (i.e. it is connected to at least one other vertex). Otherwise
 * remove the vertex and deselect the object.
 */
void Grid::updateSelectedObjOnSnap(std::shared_ptr<Vertex> &selectedObjVertexCasted)
{
    if (selectedObjVertexCasted == nullptr)
    {
        return;
    }

    selectedObj = selectedObjVertexCasted;

    if (mapData->graph.contains(selectedObjVertexCasted->getId()))
    {
        selectedObj = selectedObjVertexCasted;
    }
    else
    {
        int coords[2];
        selectedObjVertexCasted->getCoords(coords);

        mapData->vertexTree.remove(selectedObjVertexCasted->getId(), coords);

        selectedObj = nullptr;
    }
}