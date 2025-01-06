#include "EditorLayer.h"

#include <vector>

using namespace Engine;

EditorLayer::EditorLayer()
    : camera(
          {0.0f, 0.0f, 1.0f}, Application::getInstance().getWindow().getWidth(),
          Application::getInstance().getWindow().getHeight(), 1.0f, 0.5f, 3.0f),
      gridSpacing(40.0f)
{
    // Compile shaders
    lineVertexShader.addShader(ShaderType::VERTEX,
                               "res/shaders/line_vertex.vert");
    lineVertexShader.addShader(ShaderType::FRAGMENT,
                               "res/shaders/line_vertex.frag");
    lineVertexShader.compileShader();

    lineShader.addShader(ShaderType::VERTEX, "res/shaders/line.vert");
    lineShader.addShader(ShaderType::GEOMETRY, "res/shaders/line.geom");
    lineShader.addShader(ShaderType::FRAGMENT, "res/shaders/line.frag");
    lineShader.compileShader();

    phantomVertexShader.addShader(ShaderType::VERTEX,
                                  "res/shaders/phantom_vertex.vert");
    phantomVertexShader.addShader(ShaderType::FRAGMENT,
                                  "res/shaders/line_vertex.frag");
    phantomVertexShader.compileShader();

    // Add event handlers
    dispatcher.addHandler<MouseScrolledEvent>([this](MouseScrolledEvent& event)
                                              { onMouseScroll(event); });
    dispatcher.addHandler<MouseButtonPressedEvent>(
        [this](MouseButtonPressedEvent& event) { onMouseButtonPress(event); });

    dispatcher.addHandler<KeyReleasedEvent>([this](KeyReleasedEvent& event)
                                            { onKeyRelease(event); });
}

void EditorLayer::onAttach() {}

void EditorLayer::onDetach() {}

void EditorLayer::onUpdate(float deltaTime)
{
    camera.onUpdate(deltaTime);
    grid.draw(gridSpacing, camera);
    drawComponents();
    if (mode == EditorMode::INSERT) handleInsertMode();
}

void EditorLayer::onEvent(Event& event)
{
    camera.onEvent(event);
    dispatcher.dispatch(event);
}

void EditorLayer::drawComponents()
{
    // Create custom attribute layout
    CustomAttributeLayout layout;
    // Add a_IsSelected attribute
    int index = layout.addAttribute(AttributeType::INT, 1);

    // Draw lines
    Mesh lineMesh("lines", vertexVBO, lineIBO, MeshType::LINES,
                  DrawMode::STATIC, layout);
    lineMesh.attachCustomBuffer(index, selectedVertices);

    lineShader.bind();
    lineShader.setUniform1f("u_LineWeight", 4.0f * camera.getZoom());
    lineShader.setUniformMat4f("u_VP", camera.getViewProjectionMatrix());
    lineMesh.draw(lineShader);
    lineShader.unbind();

    // Draw vertices and cursor vertex
    Mesh vertexMesh("vertices", vertexVBO, vertexIBO, MeshType::POINTS,
                    DrawMode::STATIC, layout);
    vertexMesh.attachCustomBuffer(index, selectedVertices);

    lineVertexShader.bind();
    lineVertexShader.setUniformMat4f("u_VP", camera.getViewProjectionMatrix());
    vertexMesh.draw(lineVertexShader);
    lineVertexShader.unbind();
}

int EditorLayer::getVertexIndex(glm::vec2 worldPos, float threshold)
{
    for (int i = 0; i < lineVertices.size(); ++i)
    {
        const LineVertex& vertex = lineVertices.at(i);
        glm::vec2 vertexPos = {vertex.x, vertex.y};
        if (glm::distance(worldPos, vertexPos) <= threshold) return i;
    }

    return -1;
}

int EditorLayer::getLineIndex(glm::vec2 worldPos, float threshold)
{
    for (int i = 0; i < lines.size(); ++i)
    {
        const Line& line = lines.at(i);
        const LineVertex& start = lineVertices.at(line.startVertex);
        const LineVertex& end = lineVertices.at(line.endVertex);
        glm::vec2 startVertex = {start.x, start.y};
        glm::vec2 endVertex = {end.x, end.y};

        glm::vec2 startToEnd = endVertex - startVertex;
        glm::vec2 startToCursor = worldPos - startVertex;

        // Calculate the scalar projection of the cursor onto the line
        float scalar = glm::dot(startToCursor, startToEnd) /
                       glm::dot(startToEnd, startToEnd);

        // Clamp scalar to [0, 1] so that the projection is on the line
        scalar = glm::clamp(scalar, 0.0f, 1.0f);

        // Calculate the vector projection of the cursor onto the line
        glm::vec2 projection = startVertex + scalar * startToEnd;

        float distance = glm::distance(worldPos, projection);
        if (distance <= threshold) return i;
    }

    return -1;
}

int EditorLayer::getLineIndex(int vertex1, int vertex2)
{
    for (int i = 0; i < lines.size(); ++i)
    {
        const Line& line = lines.at(i);
        if ((line.startVertex == vertex1 && line.endVertex == vertex2) ||
            (line.startVertex == vertex2 && line.endVertex == vertex1))
            return i;
    }

    return -1;
}

std::vector<int> EditorLayer::getLineIndices(int vertex)
{
    std::vector<int> lineIndices;

    for (int i = 0; i < lines.size(); ++i)
    {
        const Line& line = lines.at(i);
        if (line.startVertex == vertex || line.endVertex == vertex)
            lineIndices.push_back(i);
    }

    return lineIndices;
}

int EditorLayer::getIndexInVertexVBO(int vertex)
{
    for (int i = 0; i < vertexVBO.size(); ++i)
    {
        const Vertex& v = vertexVBO.at(i);
        if (v.position.x == lineVertices.at(vertex).x &&
            v.position.y == lineVertices.at(vertex).y)
            return i;
    }

    return -1;
}

int EditorLayer::addLineVertex(float x, float y)
{
    // Check if the vertex already exists
    int index = getVertexIndex({x, y});
    if (index != -1) return index;

    // Add the vertex to the list
    int vertexIndex = getFreeVertexIndex();
    if (vertexIndex == -1)
    {
        vertexIndex = lineVertices.size();
        lineVertices.emplace_back(x, y);
    }
    else
        lineVertices.at(vertexIndex) = {x, y};

    vertexRefMap[vertexIndex] = 0;
    vertexIBO.push_back(vertexVBO.size());
    vertexVBO.push_back({{x, y, 0.0f}});
    selectedVertices.push_back(0);

    return vertexIndex;
}

int EditorLayer::addLine(int startVertex, int endVertex)
{
    ASSERT(startVertex >= 0 && startVertex < lineVertices.size(),
           "Invalid start vertex index: %d", startVertex);

    ASSERT(endVertex >= 0 && endVertex < lineVertices.size(),
           "Invalid end vertex index: %d", endVertex);

    // Check if the vertices are the same
    if (startVertex == endVertex) return -1;

    // Check if the line already exists
    if (getLineIndex(startVertex, endVertex) != -1) return -1;

    // Add the line to the list
    int lineIndex = getFreeLineIndex();
    if (lineIndex == -1)
    {
        lineIndex = lines.size();
        lines.emplace_back(startVertex, endVertex);
    }
    else
        lines.at(lineIndex) = {startVertex, endVertex};

    const LineVertex& start = lineVertices.at(startVertex);
    const LineVertex& end = lineVertices.at(endVertex);

    ++vertexRefMap[startVertex];
    int startIndex = getIndexInVertexVBO(startVertex);
    ASSERT(startIndex != -1, "Vertex %d not found in VBO", startVertex);
    lineIBO.push_back(startIndex);

    ++vertexRefMap[endVertex];
    int endIndex = getIndexInVertexVBO(endVertex);
    ASSERT(endIndex != -1, "Vertex %d not found in VBO", endVertex);
    lineIBO.push_back(endIndex);

    return lineIndex;
}

void EditorLayer::removeVertex(int index)
{
    removeVertexImpl(index);
    buildVertexVBO();
}

void EditorLayer::removeLine(int index)
{
    removeLineImpl(index);
    buildVertexVBO();
}

void EditorLayer::removeVertexImpl(int index)
{
    ASSERT(index >= 0 && index < lineVertices.size(),
           "Invalid vertex index: %d", index);

    LineVertex& vertex = lineVertices.at(index);

    std::vector<int> lineIndices = getLineIndices(index);
    for (int lineIndex : lineIndices) removeLineImpl(lineIndex);
}

void EditorLayer::removeLineImpl(int index)
{
    ASSERT(index >= 0 && index < lines.size(), "Invalid line index: %d", index);

    Line& line = lines.at(index);
    line.deleted = true;
    freeLineIndices.push(index);

    LineVertex& start = lineVertices.at(line.startVertex);
    LineVertex& end = lineVertices.at(line.endVertex);

    if (--vertexRefMap[line.startVertex] <= 0)
    {
        vertexRefMap.erase(line.startVertex);
        start.deleted = true;
        freeVertexIndices.push(line.startVertex);
    }

    if (--vertexRefMap[line.endVertex] <= 0)
    {
        vertexRefMap.erase(line.endVertex);
        end.deleted = true;
        freeVertexIndices.push(line.endVertex);
    }
}

void EditorLayer::buildVertexVBO()
{
    vertexVBO.clear();
    vertexIBO.clear();
    lineIBO.clear();

    ASSERT(vertexVBO.size() == 0, "Vertex list is not empty");
    ASSERT(vertexIBO.size() == 0, "Vertex indices is not empty");
    ASSERT(lineIBO.size() == 0, "Line indices is not empty");

    for (int i = 0; i < lineVertices.size(); ++i)
    {
        const LineVertex& lineVertex = lineVertices.at(i);
        // Skip deleted vertices
        if (lineVertex.deleted) continue;

        vertexRefMap[i] = 0;
        vertexIBO.push_back(i);
        vertexVBO.push_back({{lineVertex.x, lineVertex.y, 0.0f}});
    }

    for (const Line& line : lines)
    {
        // Skip deleted lines
        if (line.deleted) continue;

        const LineVertex& start = lineVertices.at(line.startVertex);
        const LineVertex& end = lineVertices.at(line.endVertex);

        ++vertexRefMap[line.startVertex];
        int startIndex = getIndexInVertexVBO(line.startVertex);
        ASSERT(startIndex != -1, "Vertex %d not found in VBO",
               line.startVertex);
        lineIBO.push_back(startIndex);

        ++vertexRefMap[line.endVertex];
        int endIndex = getIndexInVertexVBO(line.endVertex);
        ASSERT(endIndex != -1, "Vertex %d not found in VBO", line.endVertex);
        lineIBO.push_back(endIndex);
    }
}

int EditorLayer::getFreeVertexIndex()
{
    if (freeVertexIndices.size() > 0)
    {
        int index = freeVertexIndices.front();
        freeVertexIndices.pop();
        return index;
    }

    return -1;
}

int EditorLayer::getFreeLineIndex()
{
    if (freeLineIndices.size() > 0)
    {
        int index = freeLineIndices.front();
        freeLineIndices.pop();
        return index;
    }

    return -1;
}

void EditorLayer::handleInsertMode()
{
    if (mode != EditorMode::INSERT) return;

    glm::vec2 worldPos = camera.screenToWorld(Input::getMousePosition());
    float gridX = round(worldPos.x / gridSpacing) * gridSpacing;
    float gridY = round(worldPos.y / gridSpacing) * gridSpacing;

    if (tempStartVertex)
    {
        // Draw new line from temp start vertex to near cursor position
        Mesh phantomLineMesh("newLine",
                             {{{tempStartVertex->x, tempStartVertex->y, 0.0f}},
                              {{gridX, gridY, 0.0f}}},
                             {0, 1}, MeshType::LINES);

        lineShader.bind();
        lineShader.setUniform1f("u_LineWeight", 4.0f * camera.getZoom());
        lineShader.setUniformMat4f("u_VP", camera.getViewProjectionMatrix());
        phantomLineMesh.draw(lineShader);
        lineShader.unbind();

        // Draw temp start vertex
        Mesh tempStartVertexMesh(
            "tempStartVertex",
            {{{tempStartVertex->x, tempStartVertex->y, 0.0f}}}, {0},
            MeshType::POINTS);

        lineVertexShader.bind();
        lineVertexShader.setUniformMat4f("u_VP",
                                         camera.getViewProjectionMatrix());
        tempStartVertexMesh.draw(lineVertexShader);
        lineVertexShader.unbind();
    }

    // Draw phantom vertex
    Mesh phantomVertexMesh("phantomVertex", {{{gridX, gridY, 0.0f}}}, {0},
                           MeshType::POINTS);

    phantomVertexShader.bind();
    phantomVertexShader.setUniformMat4f("u_VP",
                                        camera.getViewProjectionMatrix());
    phantomVertexMesh.draw(phantomVertexShader);
    phantomVertexShader.unbind();
}

void EditorLayer::onMouseScroll(MouseScrolledEvent& event)
{
    if (camera.getZoom() < 2.0f)
        gridSpacing = 40.0f;
    else
        gridSpacing = 80.0f;
}

void EditorLayer::onMouseButtonPress(MouseButtonPressedEvent& event)
{
    if (mode != EditorMode::INSERT) return;

    if (event.getButton() == GLFW_MOUSE_BUTTON_LEFT)
    {
        glm::vec2 worldPos = camera.screenToWorld(Input::getMousePosition());

        float gridX = round(worldPos.x / gridSpacing) * gridSpacing;
        float gridY = round(worldPos.y / gridSpacing) * gridSpacing;

        if (!tempStartVertex)
            tempStartVertex = std::make_unique<LineVertex>(gridX, gridY);
        else
        {
            // Check if the start vertex is the same as the end vertex
            if (tempStartVertex->x != gridX || tempStartVertex->y != gridY)
            {
                int startVertex =
                    addLineVertex(tempStartVertex->x, tempStartVertex->y);
                int endVertex = addLineVertex(gridX, gridY);
                addLine(startVertex, endVertex);
            }

            tempStartVertex.reset();
        }
    }
}

void EditorLayer::onKeyRelease(KeyReleasedEvent& event)
{
    // We use key release event instead of key press event to prevent
    // multiple key presses when holding down a key

    int key = event.getKeyCode();
    switch (key)
    {
        case GLFW_KEY_E:
            if (mode != EditorMode::INSERT)
                mode = EditorMode::INSERT;
            else
            {
                mode = EditorMode::SELECT;
                tempStartVertex.reset();
            }
            break;
        case GLFW_KEY_Q:
            if (mode == EditorMode::INSERT) tempStartVertex.reset();
            mode = EditorMode::SELECT;
            break;
    }
}