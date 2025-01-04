#pragma once

/**
 * @brief A struct representing a sector.
 *
 * This struct represents a sector in the map editor.
 */
struct Sector
{
    // Height of the floor
    float floorHeight;
    // Height of the ceiling
    float ceilingHeight;
    // Light level of the sector
    int lightLevel;

    /**
     * @brief Constructs a new Sector object.
     *
     * This constructor creates a new Sector object with the specified floor
     * height, ceiling height, and light level.
     *
     * @param floorHeight The height of the floor.
     * @param ceilingHeight The height of the ceiling.
     * @param lightLevel The light level of the sector.
     */
    Sector(float floorHeight = 0, float ceilingHeight = 0, int lightLevel = 255)
        : floorHeight(floorHeight),
          ceilingHeight(ceilingHeight),
          lightLevel(lightLevel)
    {
    }
};