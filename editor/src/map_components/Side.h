#pragma once

/**
 * @brief A struct representing a side of a line.
 *
 * This struct represents a side of a line in the map editor.
 */
struct Side
{
    // Index of the sector that the side belongs to
    int sector;
    // Flag indicating whether the side is deleted
    bool deleted = false;

    /**
     * @brief Constructs a new Side object.
     *
     * This constructor creates a new Side object with the specified sector.
     *
     * @param sector The index of the sector.
     */
    Side(int sector = -1) : sector(sector) {}
};