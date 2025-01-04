#pragma once

#include <glm/glm.hpp>

namespace Engine
{
    /**
     * @brief A class that provides input functionality.
     *
     * This class provides a simple interface for getting input from the user.
     */
    class Input
    {
    public:
        Input() = delete;
        Input(const Input&) = delete;

        /**
         * @brief Checks if the specified key is pressed.
         *
         * This method checks if the specified key is pressed.
         *
         * @param key The key to check.
         * @return True if the key is pressed, false otherwise.
         */
        static bool isKeyPressed(int key);

        /**
         * @brief Checks if the specified mouse button is pressed.
         *
         * This method checks if the specified mouse button is pressed.
         *
         * @param button The mouse button to check.
         * @return True if the mouse button is pressed, false otherwise.
         */
        static bool isMouseButtonPressed(int button);

        /**
         * @brief Gets the mouse position.
         *
         * This method gets the mouse position.
         *
         * @return A pair containing the x and y coordinates of the mouse.
         */
        static glm::vec2 getMousePosition();

        /**
         * @brief Gets the x coordinate of the mouse.
         *
         * This method gets the x coordinate of the mouse.
         *
         * @return The x coordinate of the mouse.
         */
        static float getMouseX();

        /**
         * @brief Gets the y coordinate of the mouse.
         *
         * This method gets the y coordinate of the mouse.
         *
         * @return The y coordinate of the mouse.
         */
        static float getMouseY();
    };
}  // namespace Engine