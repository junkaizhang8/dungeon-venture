#include "EditorLayer.h"

using namespace Engine;

EditorLayer::EditorLayer() : camera({0.0f, 0.0f, 0.0f}, 1.0f, 1280.0f / 720.0f)
{
}

void EditorLayer::onAttach()
{
    float positions[6] = {-0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f};
    unsigned int indices[] = {0, 1, 2};

    vb = std::make_unique<VertexBuffer>(positions, sizeof(positions));
    layout = std::make_unique<VertexBufferLayout>();
    layout->push<float>(2);
    va = std::make_unique<VertexArray>();
    va->addBuffer(*vb, *layout);

    ib = std::make_unique<IndexBuffer>(indices, 3);

    shader = std::make_unique<Shader>();
    shader->addShader(Shader::Type::VERTEX, "res/shaders/default.vert");
    shader->addShader(Shader::Type::FRAGMENT, "res/shaders/default.frag");
    shader->compileShader();

    va->unbind();
    vb->unbind();
    shader->unbind();

    renderer = std::make_unique<Renderer>();
}

void EditorLayer::onDetach() {}

void EditorLayer::onUpdate(float deltaTime)
{
    renderer->clear();
    camera.onUpdate(deltaTime);
    shader->bind();
    shader->setUniformMat4f("u_MVP", camera.getViewProjectionMatrix());
    renderer->draw(*va, *ib, *shader);
}

void EditorLayer::onEvent(Event& event)
{
    camera.onEvent(event);
    dispatcher.dispatch(event);
}