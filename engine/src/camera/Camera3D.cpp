#include "Camera3D.h"

#include "core/Input.h"

namespace Engine
{
    Camera3D::Camera3D(glm::vec3 position, float width, float height,
                       glm::vec3 worldUp, float near, float far,
                       bool rotationEnabled)
        : Camera(position, width, height, rotationEnabled),
          worldUp(worldUp),
          near(near),
          far(far)
    {
        front = glm::vec3(0.0f, 0.0f, -1.0f);
        yaw = -90.0f;
        pitch = 0.0f;
        FOV = 45.0f;
        movementSpeed = 2.5f;
        mouseSensitivity = 0.1f;
        updateProjectionMatrix();
        updateViewMatrix();
        updateCameraVectors();

        dispatcher.addHandler<MouseMovedEvent>([this](MouseMovedEvent& event)
                                               { onMouseMove(event); });
    }

    void Camera3D::onUpdate(float deltaTime)
    {
        if (Input::isKeyPressed(GLFW_KEY_W))
        {
            position += movementSpeed * front;
            updateViewMatrix();
        }
        else if (Input::isKeyPressed(GLFW_KEY_S))
        {
            position -= movementSpeed * front;
            updateViewMatrix();
        }
        else if (Input::isKeyPressed(GLFW_KEY_A))
        {
            position -= movementSpeed * right;
            updateViewMatrix();
        }
        else if (Input::isKeyPressed(GLFW_KEY_D))
        {
            position += movementSpeed * right;
            updateViewMatrix();
        }
    }

    void Camera3D::updateViewMatrix()
    {
        glm::mat4 transform = glm::lookAt(position, position + front, up);
        viewMatrix = glm::inverse(transform);
        viewProjectionMatrix = projectionMatrix * viewMatrix;
    }

    void Camera3D::updateProjectionMatrix()
    {
        projectionMatrix =
            glm::perspective(glm::radians(FOV), width / height, near, far);
        viewProjectionMatrix = projectionMatrix * viewMatrix;
    }

    void Camera3D::updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(front);

        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }

    void Camera3D::onMouseMove(MouseMovedEvent& event)
    {
        if (rotationEnabled)
        {
            float xOffset = (event.getX() - prevCursorX) * mouseSensitivity;
            float yOffset = (prevCursorY - event.getY()) * mouseSensitivity;

            yaw += xOffset;
            pitch += yOffset;

            // Limit the pitch to the "top" and "bottom" of the screen
            if (pitch > 89.0f) pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;

            updateCameraVectors();
        }

        prevCursorX = event.getX();
        prevCursorY = event.getY();
    }
}  // namespace Engine