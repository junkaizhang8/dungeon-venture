#include "Grid.h"

Grid::Grid()
{
    shader.addShader(ShaderType::VERTEX, "res/shaders/grid_shader.vert");
    shader.addShader(ShaderType::GEOMETRY, "res/shaders/grid_shader.geom");
    shader.addShader(ShaderType::FRAGMENT, "res/shaders/grid_shader.frag");
    shader.compileShader();
}

void Grid::draw(const Camera2D& camera) const
{
    glm::vec3 cameraPos = camera.getPosition();
    float width = Application::getInstance().getWindow().getWidth();
    float height = Application::getInstance().getWindow().getHeight();

    float zoom = camera.getZoom();

    float gridSpacing;

    if (zoom < 0.25f)
        gridSpacing = 25.0f;
    else if (zoom < 0.5f)
        gridSpacing = 50.0f;
    else if (zoom < 2.0f)
        gridSpacing = 100.0f;
    else if (zoom < 4.0f)
        gridSpacing = 200.0f;
    else if (zoom < 8.0f)
        gridSpacing = 400.0f;
    else
        gridSpacing = 800.0f;

    float majorGridSpacing = gridSpacing * 5;

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
    CustomAttributeLayout layout;
    std::vector<float> customBuffer;

    float axisLineWeight = 1.5f * zoom;
    float majorLineWeight = 1.0f * zoom;
    float minorLineWeight = 0.5f * zoom;

    glm::vec4 axisColor = {0.8f, 0.8f, 0.8f, 1.0f};
    glm::vec4 majorColor = {0.6f, 0.6f, 0.6f, 1.0f};
    glm::vec4 minorColor = {0.5f, 0.5f, 0.5f, 1.0f};

    // Draw vertical lines
    for (float x = left; x <= right; x += gridSpacing)
    {
        // Draw axis line
        if (x == 0.0f)
        {
            vertices.push_back({{x, bottom, 0.0f}, axisColor});
            vertices.push_back({{x, top, 0.0f}, axisColor});
            customBuffer.push_back(axisLineWeight);
            customBuffer.push_back(axisLineWeight);
        }
        // Draw major grid line
        else if (fmod(x, majorGridSpacing) == 0.0f)
        {
            vertices.push_back({{x, bottom, 0.0f}, majorColor});
            vertices.push_back({{x, top, 0.0f}, majorColor});
            customBuffer.push_back(majorLineWeight);
            customBuffer.push_back(majorLineWeight);
        }
        // Draw minor grid line
        else
        {
            vertices.push_back({{x, bottom, 0.0f}, minorColor});
            vertices.push_back({{x, top, 0.0f}, minorColor});
            customBuffer.push_back(minorLineWeight);
            customBuffer.push_back(minorLineWeight);
        }
    }

    // Draw horizontal lines
    for (float y = bottom; y <= top; y += gridSpacing)
    {
        // Draw axis line
        if (y == 0.0f)
        {
            vertices.push_back({{left, y, 0.0f}, axisColor});
            vertices.push_back({{right, y, 0.0f}, axisColor});
            customBuffer.push_back(axisLineWeight);
            customBuffer.push_back(axisLineWeight);
        }
        // Draw major grid line
        else if (fmod(y, majorGridSpacing) == 0.0f)
        {
            vertices.push_back({{left, y, 0.0f}, majorColor});
            vertices.push_back({{right, y, 0.0f}, majorColor});
            customBuffer.push_back(majorLineWeight);
            customBuffer.push_back(majorLineWeight);
        }
        // Draw minor grid line
        else
        {
            vertices.push_back({{left, y, 0.0f}, minorColor});
            vertices.push_back({{right, y, 0.0f}, minorColor});
            customBuffer.push_back(minorLineWeight);
            customBuffer.push_back(minorLineWeight);
        }
    }

    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        indices.push_back(i);
    }

    layout.push(1);

    Mesh mesh("grid", vertices, indices, MeshType::LINES, DrawMode::DYNAMIC,
              CustomBufferLayoutPair(customBuffer, layout));

    shader.bind();
    shader.setUniformMat4f("u_VP", camera.getViewProjectionMatrix());
    mesh.draw(shader);
    shader.unbind();
}
