#pragma once

#include "../Selectable.h"

// Epsilon value for floating point comparison
static const float EPSILON = 0.0001f;

/**
 * @brief A struct representing a line vertex.
 *
 * This struct represents a vertex of a line in the map editor.
 */
struct LineVertex : public Selectable
{
    float x, y;
    // Flag indicating whether the vertex is deleted
    bool deleted = false;
    // Flag indicating whether the vertex is selected
    bool selected = false;

    /**
     * @brief Constructs a new LineVertex object.
     *
     * This constructor creates a new LineVertex object with the specified x and
     * y coordinates.
     *
     * @param x The x coordinate of the vertex.
     * @param y The y coordinate of the vertex.
     */
    LineVertex(float x, float y) : x(x), y(y) {}

    /**
     * @brief Checks if two LineVertex objects are equal.
     *
     * This method checks if two LineVertex objects are equal by comparing their
     * x and y coordinates.
     *
     * @param other The other LineVertex object to compare.
     * @return True if the LineVertex objects are equal, false otherwise.
     */
    bool operator==(const LineVertex& other) const
    {
        return std::abs(x - other.x) < EPSILON &&
               std::abs(y - other.y) < EPSILON;
    }
};