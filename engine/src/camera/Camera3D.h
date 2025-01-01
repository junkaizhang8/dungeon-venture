#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "events/Event.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

namespace Engine
{
    /**
     * @brief A 3D camera class.
     *
     * This class represents a 3D camera that can be used to capture 3D scenes.
     */
    class Camera3D : public Camera
    {
    public:
        /**
         * @brief Constructs a new 3D camera object.
         *
         * This constructor creates a new 3D camera object with the specified
         * position, world up vector, near and far planes, aspect ratio, and
         * rotation enabled flag.
         *
         * @param position The position of the camera.
         * @param worldUp The world up vector.
         * @param near The near plane.
         * @param far The far plane.
         * @param aspectRatio The aspect ratio of the camera.
         * @param rotationEnabled Whether or not rotation is enabled.
         */
        Camera3D(glm::vec3 position, glm::vec3 worldUp, float near, float far,
                 float aspectRatio, bool rotationEnabled = false);

        /**
         * @brief Updates the camera.
         *
         * @param deltaTime The time since the last update.
         */
        void onUpdate(float deltaTime) override;

    protected:
        /**
         * @brief Updates the view matrix.
         *
         * This method updates the view matrix of the camera.
         */
        void updateViewMatrix() override;

        /**
         * @brief Updates the projection matrix.
         *
         * This method updates the projection matrix of the camera.
         */
        void updateProjectionMatrix() override;

    private:
        // The front vector of the camera
        glm::vec3 front;
        // The up vector of the camera
        glm::vec3 up;
        // The right vector of the camera
        glm::vec3 right;
        // The world up vector
        glm::vec3 worldUp;

        // The near plane
        float near;
        // The far plane
        float far;

        // Horizontal angle
        float yaw;
        // Vertical angle
        float pitch;

        // Field of view
        float FOV;

        // Mouse sensitivity
        float mouseSensitivity;

        // Previous cursor x position
        float prevCursorX;
        // Previous cursor y position
        float prevCursorY;

        /**
         * @brief Updates the camera vectors.
         *
         * This method updates the camera vectors.
         */
        void updateCameraVectors();

        /**
         * @brief Handles a mouse move event.
         *
         * This method handles a mouse move event.
         *
         * @param event The mouse move event.
         */
        void onMouseMove(MouseMovedEvent& event);
    };
}  // namespace Engine