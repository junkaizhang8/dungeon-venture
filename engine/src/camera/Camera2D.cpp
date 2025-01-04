#include "Camera2D.h"

#include "core/Input.h"
#include "utils/EngineDebug.h"

namespace Engine
{
    Camera2D::Camera2D(glm::vec3 position, float width, float height,
                       float zoom, float minZoom, float maxZoom,
                       bool rotationEnabled)
        : Camera(position, width, height, rotationEnabled),
          zoom(zoom),
          minZoom(minZoom),
          maxZoom(maxZoom)
    {
        glm::vec2 cursorPos = Input::getMousePosition();
        prevCursorPos = {cursorPos.x, cursorPos.y};

        updateProjectionMatrix();
        updateViewMatrix();

        dispatcher.addHandler<WindowResizeEvent>(
            [this](WindowResizeEvent& event) { onWindowResize(event); });
        dispatcher.addHandler<MouseScrolledEvent>(
            [this](MouseScrolledEvent& event) { onMouseScroll(event); });
        dispatcher.addHandler<MouseMovedEvent>([this](MouseMovedEvent& event)
                                               { onMouseMove(event); });
    }

    void Camera2D::onUpdate(float deltaTime)
    {
        float adjustedSpeed = movementSpeed * zoom * deltaTime;

        // Camera movement must be updated here instead of onEvent
        // since there is a delay between initial key press and key repeat
        if (Input::isKeyPressed(GLFW_KEY_W))
        {
            position.y += adjustedSpeed;
            updateViewMatrix();
        }
        if (Input::isKeyPressed(GLFW_KEY_S))
        {
            position.y -= adjustedSpeed;
            updateViewMatrix();
        }
        if (Input::isKeyPressed(GLFW_KEY_A))
        {
            position.x -= adjustedSpeed;
            updateViewMatrix();
        }
        if (Input::isKeyPressed(GLFW_KEY_D))
        {
            position.x += adjustedSpeed;
            updateViewMatrix();
        }
    }

    glm::vec2 Camera2D::screenToWorld(glm::vec2 cursor)
    {
        // Normalize cursor position to range [0, 1]
        float normalizedX = cursor.x / width;
        // Invert Y coordinate since OpenGL has origin at bottom left
        float normalizedY = 1.0f - cursor.y / height;

        // Map to normalized device coordinates (NDC) range [-1, 1]
        float ndcX = 2.0f * normalizedX - 1.0f;
        float ndcY = 2.0f * normalizedY - 1.0f;

        // Map to world coordinates
        float worldX = ndcX * width / 2.0f * zoom + position.x;
        float worldY = ndcY * height / 2.0f * zoom + position.y;

        return {worldX, worldY};
    }

    void Camera2D::updateViewMatrix()
    {
        viewMatrix = glm::translate(glm::mat4(1.0f), -position);
        viewProjectionMatrix = projectionMatrix * viewMatrix;
    }

    void Camera2D::updateProjectionMatrix()
    {
        float halfWidth = width / 2;
        float halfHeight = height / 2;

        projectionMatrix =
            glm::ortho(-halfWidth * zoom, halfWidth * zoom, -halfHeight * zoom,
                       halfHeight * zoom, -1.0f, 1.0f);
        viewProjectionMatrix = projectionMatrix * viewMatrix;
    }

    void Camera2D::onWindowResize(WindowResizeEvent& event)
    {
        width = event.getWidth();
        height = event.getHeight();
        updateProjectionMatrix();
    }

    void Camera2D::onMouseScroll(MouseScrolledEvent& event)
    {
        zoom -= event.getYOffset() * zoomRate;
        if (zoom < minZoom) zoom = minZoom;
        if (zoom > maxZoom) zoom = maxZoom;
        updateProjectionMatrix();
    }

    void Camera2D::onMouseMove(MouseMovedEvent& event)
    {
        if (Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
        {
            float xOffset = event.getX() - prevCursorPos.x;
            float yOffset = event.getY() - prevCursorPos.y;

            position.x -= xOffset * dragSensitivity * zoom;
            position.y += yOffset * dragSensitivity * zoom;

            updateViewMatrix();
        }

        prevCursorPos = {event.getX(), event.getY()};
    }
}  // namespace Engine