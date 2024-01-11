#pragma once

typedef int vertex_id;

class Vertex
{
public:
    Vertex() : id(largestId++) {}
    Vertex(int xpos, int ypos) : id(largestId++), x(xpos), y(ypos) {}
    ~Vertex() = default;
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