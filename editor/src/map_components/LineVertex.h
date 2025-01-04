#pragma once

/**
 * @brief A struct representing a line vertex.
 *
 * This struct represents a vertex of a line in the map editor.
 */
struct LineVertex
{
    float x, y;

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
};