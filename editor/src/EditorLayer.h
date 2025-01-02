#pragma once

#include <Engine.h>

#include "Grid.h"

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
    Shader shader;
    Grid grid;
    std::unique_ptr<Mesh> mesh;
};