#pragma once

#include <Engine.h>

using namespace Engine;

class Grid
{
public:
    Grid();

    void draw(float gridSpacing, const Camera2D& camera) const;

private:
    Shader shader;
};