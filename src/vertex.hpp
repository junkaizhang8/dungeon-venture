#pragma once

#include "gridobject.hpp"
#include "renderer.hpp"

#define GRID_VERTEX_RADIUS 1
#define VERTEX_COLOUR WHITE
#define VERTEX_SELECTED_COLOUR GREEN

typedef int vertex_id;

class Vertex : public GridObject
{
public:
    Vertex() : id(largestId++) {}
    Vertex(int xpos, int ypos)
        : id(largestId++), x(xpos), y(ypos) {}
    ~Vertex() override = default;
    void drawObj(const Renderer *renderer, int gridLeft, int gridRight, int gridTop, int gridBottom) const override;
    void drawObjSelected(const Renderer *renderer, int gridLeft, int gridRight, int gridTop, int gridBottom) const override;
    vertex_id getId() const { return id; }
    void setId(vertex_id vertexId) { id = vertexId; }
    int getX() const { return x; }
    void setX(int xpos) { x = xpos; }
    int getY() const { return y; }
    void setY(int ypos) { y = ypos; }
    void getCoords(int coords[2]) const;

private:
    static vertex_id largestId;
    vertex_id id;
    int x;
    int y;
};