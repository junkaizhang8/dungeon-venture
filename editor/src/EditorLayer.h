#pragma once

#include <Engine.h>

#include <queue>
#include <unordered_map>
#include <vector>

#include "Grid.h"
#include "SelectionManager.h"
#include "map_components/Line.h"
#include "map_components/LineVertex.h"
#include "map_components/Sector.h"
#include "map_components/Side.h"

using namespace Engine;

enum class EditorMode
{
    SELECT,
    INSERT
};

class EditorLayer : public Layer
{
public:
    EditorLayer();

    void onAttach() override;

    void onDetach() override;

    void onUpdate(float deltaTime) override;

    void onEvent(Event& event) override;

private:
    EditorMode mode = EditorMode::SELECT;

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
    // The temporary start vertex when placing a new line
    std::unique_ptr<LineVertex> tempStartVertex;

    // Cached vertex list for drawing vertices
    std::vector<Vertex> vertexVBO;
    // Cached index list for drawing vertices
    std::vector<unsigned int> vertexIBO;
    // Cached index list for drawing lines
    std::vector<unsigned int> lineIBO;
    // List of selected vertices
    std::vector<int> selectedVertices;
    // A map of vertex indices to their reference count
    std::unordered_map<int, unsigned int> vertexRefMap;
    // A queue of free (deleted) vertex indices
    std::queue<int> freeVertexIndices;
    // A queue of free (deleted) line indices
    std::queue<int> freeLineIndices;

    SelectionManager selectionManager;

    // The shader used to draw the line vertices
    Shader lineVertexShader;
    // The shader used to draw the lines
    Shader lineShader;
    // The shader used to draw phantom vertices
    Shader phantomVertexShader;

    /**
     * @brief Draws the components of the map.
     *
     * This method draws the components of the map, including lines and their
     * vertices.
     */
    void drawComponents();

    /**
     * @brief Gets the index of the vertex at the specified world position
     * within the specified threshold.
     *
     * This method gets the index of the vertex at the specified world position
     * within the specified threshold. If no vertex is found, the method returns
     * -1.
     *
     * @param worldPos The world position.
     * @param threshold The threshold within which to search for a vertex.
     * Defaults to 0.
     * @return The index of the vertex, or -1 if no vertex is found.
     */
    int getVertexIndex(glm::vec2 worldPos, float threshold = 0.0f);

    /**
     * @brief Gets the index of the line at the specified world position within
     * the specified threshold.
     *
     * This method gets the index of the line at the specified world position
     * within the specified threshold. If no line is found, the method returns
     * -1.
     *
     * @param worldPos The world position.
     * @param threshold The threshold within which to search for a line.
     * Defaults to 0.
     * @return The index of the line, or -1 if no line is found.
     */
    int getLineIndex(glm::vec2 worldPos, float threshold = 0.0f);

    /**
     * @brief Gets the index of the line that has the specified two vertices
     * as endpoints.
     *
     * This method gets the index of the line that has the specified two
     * vertices as endpoints.
     *
     * @param vertex1 The index of the first vertex.
     * @param vertex2 The index of the second vertex.
     * @return The index of the line, or -1 if no line is found.
     */
    int getLineIndex(int vertex1, int vertex2);

    /**
     * @brief Gets the indices of the lines that have the specified vertex as
     * an endpoint.
     *
     * This method gets the indices of the lines that have the specified vertex
     * as an endpoint.
     *
     * @param vertex The index of the vertex.
     * @return A vector containing the indices of the lines.
     */
    std::vector<int> getLineIndices(int vertex);

    /**
     * @brief Gets the index of the specified vertex in the vertex VBO.
     *
     * This method gets the index of the specified vertex in the vertex VBO.
     *
     * @param vertex The index of the vertex.
     * @return The index of the vertex in the VBO, or -1 if the vertex is not
     * found.
     */
    int getIndexInVertexVBO(int vertex);

    /**
     * @brief Adds a line vertex at the specified position.
     *
     * This method adds a line vertex at the specified position. If a vertex
     * already exists at the position, the method returns the index of the
     * existing vertex.
     *
     * @param x The x coordinate of the vertex.
     * @param y The y coordinate of the vertex.
     *
     * @return The index of the vertex.
     */
    int addLineVertex(float x, float y);

    /**
     * @brief Adds a line with the specified start and end vertices.
     *
     * This method adds a line with the specified start and end vertices. If the
     * vertices are the same or the line already exists, the method returns -1.
     * For efficiency, always call this function AFTER calling addVertex on the
     * two vertices so that checking if the vertices are in the vertex VBO is
     * unnecessary.
     *
     * @param startVertex The index of the start vertex.
     * @param endVertex The index of the end vertex.
     *
     * @return The index of the line.
     */
    int addLine(int startVertex, int endVertex);

    /**
     * @brief Removes the vertex at the specified index and rebuilds the buffer
     * objects.
     *
     * This method removes the vertex at the specified index and rebuilds the
     * vertex and line buffer objects.
     *
     * @param index The index of the vertex to remove.
     */
    void removeVertex(int index);

    /**
     * @brief Removes the line at the specified index and rebuilds the buffer
     * objects.
     *
     * This method removes the line at the specified index and rebuilds the
     * vertex and line buffer objects.
     *
     * @param index The index of the line to remove.
     */
    void removeLine(int index);

    /**
     * @brief Removes the vertex at the specified index.
     *
     * This method removes the vertex at the specified index.
     *
     * @param index The index of the vertex to remove.
     */
    void removeVertexImpl(int index);

    /**
     * @brief Removes the line at the specified index.
     *
     * This method removes the line at the specified index.
     *
     * @param index The index of the line to remove.
     */
    void removeLineImpl(int index);

    /**
     * @brief Builds the vertex VBO.
     *
     * This method builds the vertex VBO and the IBOs for the vertices and
     * lines.
     */
    void buildVertexVBO();

    /**
     * @brief Gets the index of a free vertex in the vertex list.
     *
     * This method gets the index of a free vertex in the vertex list, or -1 if
     * no free vertex is found.
     *
     * @return The index of a free vertex.
     */
    int getFreeVertexIndex();

    /**
     * @brief Gets the index of a free line in the line list.
     *
     * This method gets the index of a free line in the line list, or -1 if no
     * free line is found.
     *
     * @return The index of a free line.
     */
    int getFreeLineIndex();

    /**
     * @brief Handles the select mode.
     *
     * This method handles the select mode.
     */
    void handleSelectMode();

    /**
     * @brief Handles the insert mode.
     *
     * This method handles the insert mode.
     */
    void handleInsertMode();

    /**
     * @brief Handles the mouse scroll event.
     *
     * This method handles the mouse scroll event.
     *
     * @param event The mouse scroll event.
     */
    void onMouseScroll(MouseScrolledEvent& event);

    /**
     * @brief Handles the mouse button press event.
     *
     * This method handles the mouse button press event.
     *
     * @param event The mouse button press event.
     */
    void onMouseButtonPress(MouseButtonPressedEvent& event);

    /**
     * @brief Handles the key release event.
     *
     * This method handles the key release event.
     *
     * @param event The key release event.
     */
    void onKeyRelease(KeyReleasedEvent& event);
};