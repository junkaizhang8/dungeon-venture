#pragma once

#include <Engine.h>

#include "Grid.h"
#include "map_components/Line.h"
#include "map_components/LineVertex.h"
#include "map_components/Sector.h"
#include "map_components/Side.h"

using namespace Engine;

class EditorLayer : public Layer
{
public:
    EditorLayer();

    void onAttach() override;

    void onDetach() override;

    void onUpdate(float deltaTime) override;

    void onEvent(Event& event) override;

private:
    // The camera used to view the scene
    Camera2D camera;
    // The grid that is drawn in the scene
    Grid grid;
    // The spacing between grid lines
    float gridSpacing;
    // The list of line vertices in the map
    std::vector<LineVertex> lineVertices;
    // The list of lines in the map
    std::vector<Line> lines;
    // The list of sides in the map
    std::vector<Side> sides;
    // The list of sectors in the map
    std::vector<Sector> sectors;
    // The shader used to draw the line vertices
    Shader lineVertexShader;
    // The shader used to draw the lines
    Shader lineShader;

    /**
     * @brief Draws the components of the map.
     *
     * This method draws the components of the map, including lines and their
     * vertices.
     */
    void drawComponents();

    /**
     * @brief Handles the mouse scroll event.
     *
     * This method handles the mouse scroll event.
     *
     * @param event The mouse scroll event.
     */
    void onMouseScroll(MouseScrolledEvent& event);
};