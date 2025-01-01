#include "Camera2D.h"

#include "core/Input.h"
#include "utils/EngineDebug.h"

namespace Engine
{
    Camera2D::Camera2D(glm::vec3 position, float zoom, float aspectRatio,
                       bool rotationEnabled)
        : Camera(position, aspectRatio, rotationEnabled),
          zoom(zoom),
          zoomRate(0.1f),
          dragSensitivity(0.005f)
    {
        std::pair<float, float> cursorPos = Input::getMousePosition();
        prevCursorX = cursorPos.first;
        prevCursorY = cursorPos.second;

        updateMovementSpeed();

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
        // Camera movement must be updated here instead of onEvent
        // since there is a delay between initial key press and key repeat
        if (Input::isKeyPressed(GLFW_KEY_W))
        {
            position.y += movementSpeed * deltaTime;
            updateViewMatrix();
        }
        if (Input::isKeyPressed(GLFW_KEY_S))
        {
            position.y -= movementSpeed * deltaTime;
            updateViewMatrix();
        }
        if (Input::isKeyPressed(GLFW_KEY_A))
        {
            position.x -= movementSpeed * deltaTime;
            updateViewMatrix();
        }
        if (Input::isKeyPressed(GLFW_KEY_D))
        {
            position.x += movementSpeed * deltaTime;
            updateViewMatrix();
        }
    }

    void Camera2D::updateViewMatrix()
    {
        glm::mat4 transform =
            glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, -1.0f),
                        glm::vec3(0.0f, 1.0f, 0.0f));
        viewMatrix = glm::inverse(transform);
        viewProjectionMatrix = projectionMatrix * viewMatrix;
    }

    void Camera2D::updateProjectionMatrix()
    {
        projectionMatrix = glm::ortho(-aspectRatio * zoom, aspectRatio * zoom,
                                      -zoom, zoom, -1.0f, 1.0f);
        viewProjectionMatrix = projectionMatrix * viewMatrix;
    }

    void Camera2D::updateMovementSpeed() { movementSpeed = zoom; }

    void Camera2D::onWindowResize(WindowResizeEvent& event)
    {
        aspectRatio = static_cast<float>(event.getWidth()) / event.getHeight();
        updateProjectionMatrix();
    }

    void Camera2D::onMouseScroll(MouseScrolledEvent& event)
    {
        zoom -= event.getYOffset() * zoomRate;
        if (zoom < 0.1f) zoom = 0.1f;
        if (zoom > 10.0f) zoom = 10.0f;
        updateMovementSpeed();
        updateProjectionMatrix();
    }

    void Camera2D::onMouseMove(MouseMovedEvent& event)
    {
        if (Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
        {
            float xOffset = event.getX() - prevCursorX;
            float yOffset = event.getY() - prevCursorY;

            position.x += xOffset * dragSensitivity * zoom;
            position.y -= yOffset * dragSensitivity * zoom;

            updateViewMatrix();
        }

        prevCursorX = event.getX();
        prevCursorY = event.getY();
    }
}  // namespace Engine