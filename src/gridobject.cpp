#include "gridobject.hpp"

void GridObject::getObjPosRelativeToWindow(int coords[2], int gridLeft, int gridRight,
                                           int gridTop, int gridBottom) const
{
    coords[0] += (gridRight - gridLeft) / 2;
    coords[1] += (gridBottom - gridTop) / 2;
}