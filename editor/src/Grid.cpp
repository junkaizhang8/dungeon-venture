#include "Grid.h"

Grid::Grid()
{
    shader.addShader(ShaderType::VERTEX, "res/shaders/grid_shader.vert");
    shader.addShader(ShaderType::FRAGMENT, "res/shaders/grid_shader.frag");
    shader.compileShader();
}

void Grid::draw(const Camera2D& camera) const
{
    glm::vec3 cameraPos = camera.getPosition();
    float width = Application::getInstance().getWindow().getWidth();
    float height = Application::getInstance().getWindow().getHeight();

    float zoom = camera.getZoom();
    float gridSpacing = 100.0f;

    // Calculate the left, right, bottom, and top of the grid
    // Add gridSpacing to the bounds to ensure the grid spans the entire screen
    float leftUnmodded = cameraPos.x - (width / 2) * zoom;
    float left = leftUnmodded - fmod(leftUnmodded, gridSpacing) - gridSpacing;
    float rightUnmodded = cameraPos.x + (width / 2) * zoom;
    float right = rightUnmodded + gridSpacing -
                  fmod(rightUnmodded, gridSpacing) + gridSpacing;
    float bottomUnmodded = cameraPos.y - (height / 2) * zoom;
    float bottom =
        bottomUnmodded - fmod(bottomUnmodded, gridSpacing) - gridSpacing;
    float topUnmodded = cameraPos.y + (height / 2) * zoom;
    float top = topUnmodded + gridSpacing - fmod(topUnmodded, gridSpacing) +
                gridSpacing;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // Draw vertical lines
    for (float x = left; x <= right; x += gridSpacing)
    {
        vertices.push_back({{x, bottom, 0.0f}});
        vertices.push_back({{x, top, 0.0f}});
    }

    // Draw horizontal lines
    for (float y = bottom; y <= top; y += gridSpacing)
    {
        vertices.push_back({{left, y, 0.0f}});
        vertices.push_back({{right, y, 0.0f}});
    }

    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        indices.push_back(i);
    }

    Mesh mesh("grid", vertices, indices, MeshType::LINES, DrawMode::DYNAMIC);

    shader.bind();
    shader.setUniformMat4f("u_MVP", camera.getViewProjectionMatrix());
    mesh.draw(shader);
    shader.unbind();
}