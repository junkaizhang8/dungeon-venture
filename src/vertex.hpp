#pragma once

class Vertex
{
public:
    Vertex() = default;
    Vertex(int xpos, int ypos);
    ~Vertex() = default;
    int getX() { return x; }
    void setX(int xpos) { x = xpos; }
    int getY() { return y; }
    void setY(int ypos) { y = ypos; }
    void getCoords(double coords[2]);

private:
    int x;
    int y;
};