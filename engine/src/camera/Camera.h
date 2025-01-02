#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "events/ApplicationEvent.h"
#include "events/Event.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

namespace Engine
{
    /**
     * @brief A camera class.
     *
     * This class represents a camera that can be used to capture scenes.
     */
    class Camera
    {
    public:
        /**
         * @brief Constructs a new camera object.
         *
         * This constructor creates a new camera object with the specified
         * position, aspect ratio, and rotation enabled flag.
         *
         * @param position The position of the camera.
         * @param width The width of the window.
         * @param height The height of the window.
         * @param rotationEnabled Whether or not rotation is enabled.
         */
        Camera(glm::vec3 position, float width, float height,
               bool rotationEnabled = false);

        /**
         * @brief Updates the camera.
         *
         * This method updates the camera.
         *
         * @param deltaTime The time since the last update.
         */
        virtual void onUpdate(float deltaTime) = 0;

        /**
         * @brief Handles an event.
         *
         * This method handles an event.
         *
         * @param event The event to handle.
         */
        void onEvent(Event& event);

        /**
         * @brief Gets the view matrix.
         *
         * This method returns the view matrix of the camera.
         *
         * @return The view matrix of the camera.
         */
        inline glm::mat4 getViewMatrix() const { return viewMatrix; }

        /**
         * @brief Gets the projection matrix.
         *
         * This method returns the projection matrix of the camera.
         *
         * @return The projection matrix of the camera.
         */
        inline glm::mat4 getProjectionMatrix() const
        {
            return projectionMatrix;
        }

        /**
         * @brief Gets the view-projection matrix.
         *
         * This method returns the view-projection matrix of the camera.
         *
         * @return The view-projection matrix of the camera.
         */
        inline glm::mat4 getViewProjectionMatrix() const
        {
            return viewProjectionMatrix;
        }

        /**
         * @brief Gets the position of the camera.
         *
         * This method returns the position of the camera.
         *
         * @return The position of the camera.
         */
        inline glm::vec3 getPosition() const { return position; }

        inline void setPosition(const glm::vec3& position)
        {
            this->position = position;
        }

        /**
         * @brief Sets the movement speed of the camera.
         *
         * This method sets the movement speed of the camera.
         *
         * @param speed The movement speed of the camera.
         */
        inline void setMovementSpeed(float speed) { movementSpeed = speed; }

        /**
         * @brief Sets the aspect ratio of the camera.
         *
         * This method sets the aspect ratio of the camera.
         *
         * @param aspectRatio The aspect ratio of the camera.
         */
        inline void setRotationEnabled(bool enabled)
        {
            rotationEnabled = enabled;
        }

    protected:
        // The position of the camera
        glm::vec3 position;

        // Camera speed
        float movementSpeed = 400.0f;

        // The width of the window
        float width;
        // The height of the window
        float height;

        // Whether or not rotation is enabled
        bool rotationEnabled;

        // The event dispatcher
        EventDispatcher dispatcher;

        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
        glm::mat4 viewProjectionMatrix;

        /**
         * @brief Updates the view matrix.
         *
         * This method updates the view matrix of the camera.
         */
        virtual void updateViewMatrix() = 0;

        /**
         * @brief Updates the projection matrix.
         *
         * This method updates the projection matrix of the camera.
         */
        virtual void updateProjectionMatrix() = 0;
    };
}  // namespace Engine