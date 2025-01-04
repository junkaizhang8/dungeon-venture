#include "EditorLayer.h"

#include <vector>

using namespace Engine;

EditorLayer::EditorLayer()
    : camera(
          {0.0f, 0.0f, 1.0f}, Application::getInstance().getWindow().getWidth(),
          Application::getInstance().getWindow().getHeight(), 1.0f, 0.5f, 3.0f),
      gridSpacing(40.0f)
{
    lineVertexShader.addShader(ShaderType::VERTEX,
                               "res/shaders/line_vertex.vert");
    lineVertexShader.addShader(ShaderType::FRAGMENT,
                               "res/shaders/line_vertex.frag");
    lineVertexShader.compileShader();
    lineVertexShader.unbind();

    lineShader.addShader(ShaderType::VERTEX, "res/shaders/line.vert");
    lineShader.addShader(ShaderType::GEOMETRY, "res/shaders/line.geom");
    lineShader.addShader(ShaderType::FRAGMENT, "res/shaders/line.frag");
    lineShader.compileShader();
    lineShader.unbind();

    // Temporary line for demo
    lineVertices.emplace_back(-80.f, -80.f);
    lineVertices.emplace_back(80.f, -80.f);
    lines.emplace_back(0, 1);

    // Add event handlers
    dispatcher.addHandler<MouseScrolledEvent>([this](MouseScrolledEvent& event)
                                              { onMouseScroll(event); });
}

void EditorLayer::onAttach() {}

void EditorLayer::onDetach() {}

void EditorLayer::onUpdate(float deltaTime)
{
    camera.onUpdate(deltaTime);
    grid.draw(gridSpacing, camera);
    drawComponents();
}

void EditorLayer::onEvent(Event& event)
{
    camera.onEvent(event);
    dispatcher.dispatch(event);
}

void EditorLayer::drawComponents()
{
    std::vector<Vertex> lineList;
    std::vector<unsigned int> lineIndices;
    glm::vec4 lineColor = {1.0f, 1.0f, 1.0f, 1.0f};

    for (const Line& line : lines)
    {
        const LineVertex& start = lineVertices.at(line.startVertex);
        const LineVertex& end = lineVertices.at(line.endVertex);

        lineList.push_back({{start.x, start.y, 0.0f}, lineColor});
        lineList.push_back({{end.x, end.y, 0.0f}, lineColor});
        lineIndices.push_back(lineIndices.size());
        lineIndices.push_back(lineIndices.size());
    }

    std::vector<Vertex> vertexList;
    std::vector<unsigned int> vertexIndices;
    glm::vec4 vertexColor = {1.0f, 1.0f, 0.0f, 1.0f};

    for (const LineVertex& lineVertex : lineVertices)
    {
        vertexList.push_back({{lineVertex.x, lineVertex.y, 0.0f}, vertexColor});
        vertexIndices.push_back(vertexIndices.size());
    }

    Mesh lineMesh("lines", lineList, lineIndices, MeshType::LINES);
    Mesh vertexMesh("vertices", vertexList, vertexIndices, MeshType::POINTS);

    lineShader.bind();
    lineShader.setUniform1f("u_LineWeight", 3.0f * camera.getZoom());
    lineShader.setUniformMat4f("u_VP", camera.getViewProjectionMatrix());
    lineMesh.draw(lineShader);
    lineShader.unbind();

    lineVertexShader.bind();
    lineVertexShader.setUniformMat4f("u_VP", camera.getViewProjectionMatrix());
    vertexMesh.draw(lineVertexShader);
    lineVertexShader.unbind();
}

void EditorLayer::onMouseScroll(MouseScrolledEvent& event)
{
    if (camera.getZoom() < 2.0f)
        gridSpacing = 40.0f;
    else
        gridSpacing = 80.0f;
}