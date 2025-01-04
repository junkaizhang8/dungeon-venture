#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "events/Event.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "utils/EngineDebug.h"

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
         * position, width, height, zoom level, minimum zoom level, maximum zoom
         * level, and rotation flag.
         *
         * @param position The position of the camera.
         * @param width The width of the camera.
         * @param height The height of the camera.
         * @param zoom The zoom level of the camera.
         * @param minZoom The minimum zoom level of the camera.
         * @param maxZoom The maximum zoom level of the camera.
         * @param rotationEnabled A flag indicating whether rotation is enabled.
         */
        Camera2D(glm::vec3 position, float width, float height, float zoom,
                 float minZoom = 0.1f, float maxZoom = 10.0f,
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
         * @brief Gets the zoom level of the camera.
         *
         * @return The zoom level of the camera.
         */
        inline float getZoom() const { return zoom; }

        /**
         * @brief Sets the zoom rate of the camera.
         *
         * This method sets the zoom rate of the camera.
         *
         * @param rate The zoom rate of the camera.
         */
        inline void setZoomRate(float rate) { zoomRate = rate; }

        /**
         * @brief Gets the minimum zoom level of the camera.
         *
         * @return The minimum zoom level of the camera.
         */
        inline float getMinZoom() const { return minZoom; }

        /**
         * @brief Sets the minimum zoom level of the camera.
         *
         * This method sets the minimum zoom level of the camera.
         *
         * @param min The minimum zoom level of the camera.
         */
        inline void setMinZoom(float min)
        {
            ENGINE_ASSERT(min > 0, "Minimum zoom must be greater than 0");
            ENGINE_ASSERT(min < maxZoom,
                          "Minimum zoom must be less than max zoom");
            minZoom = min;
        }

        /**
         * @brief Gets the maximum zoom level of the camera.
         *
         * @return The maximum zoom level of the camera.
         */
        inline float getMaxZoom() const { return maxZoom; }

        /**
         * @brief Sets the maximum zoom level of the camera.
         *
         * This method sets the maximum zoom level of the camera.
         *
         * @param max The maximum zoom level of the camera.
         */
        inline void setMaxZoom(float max)
        {
            ENGINE_ASSERT(max > 0, "Maximum zoom must be greater than 0");
            ENGINE_ASSERT(max > minZoom,
                          "Maximum zoom must be greater than min zoom");
            maxZoom = max;
        }

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
        // The zoom level of the camera (higher zoom = farther away)
        float zoom;
        // The zoom rate of the camera
        float zoomRate = 0.1f;
        // The minimum level of zoom
        float minZoom;
        // The maximum level of zoom
        float maxZoom;

        // Drag sensitivity of the camera
        float dragSensitivity = 1.0f;

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