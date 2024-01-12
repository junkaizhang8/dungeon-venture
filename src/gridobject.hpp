#pragma once

#include <memory>
#include "renderer.hpp"

class GridObject
{
public:
    GridObject() = default;
    virtual ~GridObject() = default;
    virtual void drawObj(std::shared_ptr<Renderer> renderer, int gridLeft,\
                         int gridRight, int gridTop, int gridBottom) = 0;
    virtual void drawObjSelected(std::shared_ptr<Renderer> renderer, int gridLeft,\
                                 int gridRight, int gridTop, int gridBottom) = 0;
    void getObjPosRelativeToWindow(int coords[2], int gridLeft, int gridRight,\
                                   int gridTop, int gridBottom);
};