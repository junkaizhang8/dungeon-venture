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
}

void Grid::drawNewWall()
{
    int cursorX;
    int cursorY;
    int onClickCursorX;
    int onClickCursorY;

    Cursor::getScaledOnLeftClickPos(onClickCursorX, onClickCursorY);

    if (!inGrid(onClickCursorX, onClickCursorY))
    {
        return;
    }

    Cursor::getScaledCursorPos(cursorX, cursorY);

    if (Cursor::leftButtonPressed())
    {
        if (cursorX < gridLeft)
        {
            cursorX = gridLeft;
        }

        if (cursorX > gridRight)
        {
            cursorX = gridRight;
        }

        if (cursorY < gridTop)
        {
            cursorY = gridTop;
        }

        if (cursorY > gridBottom)
        {
            cursorY = gridBottom;
        }

        windowCoordToGridCoord(cursorX, cursorY);

        std::shared_ptr<Vertex> neighbourVertex;
        int coords[2] = {cursorX, cursorY};

        if (newWall.getStartVertex() == nullptr)
        {
            if ((neighbourVertex = tree.proximitySearch(coords, VERTEX_SNAP_DIST)))
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

        drawWall(newWall);
    } else
    {
        if (!newWall.getStartVertex() || !newWall.getEndVertex())
        {
            return;
        }

        std::shared_ptr<Vertex> neighbourVertex;
        int coords[2];

        newWall.getEndVertex().get()->getCoords(coords);

        if ((neighbourVertex = tree.proximitySearch(coords, VERTEX_SNAP_DIST)))
        {
            newWall.setEndVertex(neighbourVertex);
        }

        int startCoords[2];
        int endCoords[2];

        newWall.getStartVertex().get()->getCoords(startCoords);
        newWall.getEndVertex().get()->getCoords(endCoords);

        if (!sameCoords(startCoords, endCoords))
        {
            tree.insert(newWall.getStartVertex());
            tree.insert(newWall.getEndVertex());
            wallList.push_back(newWall);
            graph.insertMapping(newWall);
        }

        Wall wall;
        newWall = wall;
    }
}

void Grid::cancelNewWall()
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

void Grid::drawWalls()
{
    for (Wall wall : wallList)
    {
        drawWall(wall);
    }
}

void Grid::drawWall(Wall &wall)
{
    int startCoords[2] = {};
    int endCoords[2] = {};

    wall.getStartVertex().get()->getCoords(startCoords);
    wall.getEndVertex().get()->getCoords(endCoords);
    gridCoordToWindowCoord(startCoords[0], startCoords[1]);
    gridCoordToWindowCoord(endCoords[0], endCoords[1]);

    double wallWidth = GRID_WALL_WIDTH;
    double wallWidthHalf = wallWidth / 2;
    double theta = atan((double)(startCoords[1] - endCoords[1]) / (double)(startCoords[0] - endCoords[0]));
    double xOffset = wallWidthHalf * sin(theta);
    double yOffset = wallWidthHalf * cos(theta);

    double vertices[4][2] =
    {
        {startCoords[0] - xOffset, startCoords[1] + yOffset},
        {startCoords[0] + xOffset, startCoords[1] - yOffset},
        {endCoords[0] + xOffset, endCoords[1] - yOffset},
        {endCoords[0] - xOffset, endCoords[1] + yOffset}
    };

    renderer.get()->drawQuad(vertices, YELLOW);

    renderer.get()->drawFilledCircle(startCoords[0], startCoords[1], GRID_VERTEX_RADIUS, WHITE);
    renderer.get()->drawFilledCircle(endCoords[0], endCoords[1], GRID_VERTEX_RADIUS, WHITE);
}

bool Grid::inGrid(int x, int y)
{
    return (x >= gridLeft) && (x <= gridRight) && (y >= gridTop) && (y <= gridBottom);
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

bool Grid::sameCoords(const int coords1[2], const int coords2[2])
{
    return (coords1[0] == coords2[0]) && (coords1[1] == coords2[1]);
}