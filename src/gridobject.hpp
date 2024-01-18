#pragma once

#include <memory>
#include <iostream>
#include "renderer.hpp"

/*
 * Abstract representation of a grid object
 */
class GridObject
{
public:
    GridObject() = default;
    virtual ~GridObject() = default;
    virtual void drawObj(const Renderer *const renderer, int gridLeft, int gridRight,
                         int gridTop, int gridBottom) const = 0;
    virtual void drawObjSelected(const Renderer *const renderer, int gridLeft, int gridRight,
                                 int gridTop, int gridBottom) const = 0;
    void getObjPosRelativeToWindow(int coords[2], int gridLeft, int gridRight,
                                   int gridTop, int gridBottom) const;
};