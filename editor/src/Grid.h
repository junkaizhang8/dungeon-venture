#pragma once

#include <Engine.h>

using namespace Engine;

class Grid
{
public:
    Grid();

    void draw(const Camera2D& camera) const;

private:
    Shader shader;
};