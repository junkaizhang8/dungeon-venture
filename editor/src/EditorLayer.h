#pragma once

#include <Engine.h>

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
    Camera2D camera;
    std::unique_ptr<VertexBuffer> vb;
    std::unique_ptr<VertexBufferLayout> layout;
    std::unique_ptr<VertexArray> va;
    std::unique_ptr<IndexBuffer> ib;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Texture> texture;
};