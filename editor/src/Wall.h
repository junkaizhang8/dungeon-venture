#pragma once

#include <glm/glm.hpp>

class WallVertex
{
public:
    WallVertex(float x, float y);

    inline const glm::vec2& getPosition() const { return position; }

    inline void setPosition(const glm::vec2& position)
    {
        this->position = position;
    }

    void draw() const;

private:
    glm::vec2 position;
};