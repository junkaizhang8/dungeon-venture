#pragma once

#include "../Selectable.h"

/**
 * @brief A struct representing a line.
 *
 * This struct represents a line in the map editor.
 */
struct Line : public Selectable
{
    // Index of the start vertex
    int startVertex;
    // Index of the end vertex
    int endVertex;
    // Index of the front side
    int front;
    // Index of the back side
    int back;
    // Flag indicating whether the line is deleted
    bool deleted = false;
    // Flag indicating whether the line is selected
    bool selected = false;

    /**
     * @brief Constructs a new Line object.
     *
     * This constructor creates a new Line object with the specified start and
     * end vertices, and front and back sides.
     *
     * @param startVertex The index of the start vertex.
     * @param endVertex The index of the end vertex.
     * @param front The index of the front side.
     * @param back The index of the back side.
     */
    Line(int startVertex, int endVertex, int front = -1, int back = -1)
        : startVertex(startVertex),
          endVertex(endVertex),
          front(front),
          back(back)
    {
    }
};