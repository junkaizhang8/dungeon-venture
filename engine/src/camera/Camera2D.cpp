#include "Camera2D.h"

#include "core/Input.h"
#include "utils/EngineDebug.h"

namespace Engine
{
    Camera2D::Camera2D(glm::vec3 position, float zoom, float width,
                       float height, bool rotationEnabled)
        : Camera(position, width, height, rotationEnabled), zoom(zoom)
    {
        std::pair<float, float> cursorPos = Input::getMousePosition();
        prevCursorX = cursorPos.first;
        prevCursorY = cursorPos.second;

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
            float xOffset = event.getX() - prevCursorX;
            float yOffset = event.getY() - prevCursorY;

            position.x -= xOffset * dragSensitivity * zoom;
            position.y += yOffset * dragSensitivity * zoom;

            updateViewMatrix();
        }

        prevCursorX = event.getX();
        prevCursorY = event.getY();
    }
}  // namespace Engine