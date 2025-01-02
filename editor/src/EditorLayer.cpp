#include "EditorLayer.h"

using namespace Engine;

EditorLayer::EditorLayer()
    : camera({0.0f, 0.0f, 1.0f}, 1.0f,
             Application::getInstance().getWindow().getWidth(),
             Application::getInstance().getWindow().getHeight())
{
}

void EditorLayer::onAttach()
{
    std::vector<Vertex> vertices = {
        {{-200.0f, -200.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        {{200.0f, -200.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
        {{200.0f, 200.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}};
    std::vector<unsigned int> indices = {0, 1, 2};

    mesh =
        std::make_unique<Mesh>("quad", vertices, indices, MeshType::TRIANGLES);

    shader.addShader(ShaderType::VERTEX, "res/shaders/default.vert");
    shader.addShader(ShaderType::FRAGMENT, "res/shaders/default.frag");
    shader.compileShader();

    shader.unbind();
}

void EditorLayer::onDetach() {}

void EditorLayer::onUpdate(float deltaTime)
{
    camera.onUpdate(deltaTime);
    grid.draw(camera);
    shader.bind();
    shader.setUniformMat4f("u_MVP", camera.getViewProjectionMatrix());
    mesh->draw(shader);
    shader.unbind();
}

void EditorLayer::onEvent(Event& event)
{
    camera.onEvent(event);
    dispatcher.dispatch(event);
}