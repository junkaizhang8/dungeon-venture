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
    Vertex(int xpos, int ypos) : id(largestId++), x(xpos), y(ypos) {}
    ~Vertex() override = default;
    void drawObj(std::shared_ptr<Renderer> renderer, int gridLeft, int gridRight, int gridTop, int gridBottom) override;
    void drawObjSelected(std::shared_ptr<Renderer> renderer, int gridLeft, int gridRight, int gridTop, int gridBottom) override;
    vertex_id getId() { return id; }
    int getX() { return x; }
    void setX(int xpos) { x = xpos; }
    int getY() { return y; }
    void setY(int ypos) { y = ypos; }
    void getCoords(int coords[2]);

private:
    static vertex_id largestId;
    vertex_id id;
    int x;
    int y;
};