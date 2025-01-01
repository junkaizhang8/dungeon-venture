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
     * @brief A 2D camera class.
     *
     * This class represents a 2D camera that can be used to capture 2D scenes.
     */
    class Camera2D : public Camera
    {
    public:
        /**
         * @brief Constructs a new 2D camera object.
         *
         * This constructor creates a new 2D camera object with the specified
         * position, zoom level, aspect ratio, and rotation enabled flag.
         *
         * @param position The position of the camera.
         * @param zoom The zoom level of the camera.
         * @param aspectRatio The aspect ratio of the camera.
         * @param rotationEnabled Whether or not rotation is enabled.
         */
        Camera2D(glm::vec3 position, float zoom, float aspectRatio,
                 bool rotationEnabled = false);

        /**
         * @brief Updates the camera.
         *
         * This method updates the camera.
         *
         * @param deltaTime The time since the last update.
         */
        void onUpdate(float deltaTime) override;

        /**
         * @brief Sets the zoom rate of the camera.
         *
         * This method sets the zoom rate of the camera.
         *
         * @param rate The zoom rate of the camera.
         */
        inline float setZoomRate(float rate) { return zoomRate = rate; }

        /**
         * @brief Sets the drag sensitivity of the camera.
         *
         * This method sets the drag sensitivity of the camera.
         *
         * @param sensitivity The drag sensitivity of the camera.
         */
        inline float setDragSensitivity(float sensitivity)
        {
            return dragSensitivity = sensitivity;
        }

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
        // The zoom level of the camera
        float zoom;
        // The zoom rate of the camera
        float zoomRate;

        // Drag sensitivity of the camera
        float dragSensitivity;

        // Previous cursor x position
        float prevCursorX;
        // Previous cursor y position
        float prevCursorY;

        /**
         * @brief Updates the movement speed of the camera.
         *
         * This method updates the movement speed of the camera based on the
         * current zoom level.
         */
        void updateMovementSpeed();

        /**
         * @brief Handles the window resize event.
         *
         * This method handles the window resize event.
         *
         * @param event The window resize event.
         */
        void onWindowResize(WindowResizeEvent& event);

        /**
         * @brief Handles the mouse scroll event.
         *
         * This method handles the mouse scroll event.
         *
         * @param event The mouse scroll event.
         */
        void onMouseScroll(MouseScrolledEvent& event);

        /**
         * @brief Handles the mouse move event.
         *
         * This method handles the mouse move event.
         *
         * @param event The mouse move event.
         */
        void onMouseMove(MouseMovedEvent& event);
    };
}  // namespace Engine