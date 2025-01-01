#pragma once

#include <string>

#include "Renderer.h"

namespace Engine
{

    /**
     * @brief A class that encapsulates an OpenGL texture object.
     *
     * This class provides a simple interface for creating and managing texture
     * objects in OpenGL.
     */
    class Texture
    {
    public:
        /**
         * @brief Creates a new Texture object.
         *
         * This constructor creates a new Texture object and initializes it with
         * the texture data loaded from the image file at the specified path.
         *
         * @param filepath The path to the image file to load.
         */
        Texture(const std::string& filepath);

        /**
         * @brief Destroys the Texture object.
         *
         * This destructor destroys the Texture object and frees any resources
         * associated with it.
         */
        ~Texture();

        /**
         * @brief Binds the Texture to the current OpenGL context.
         *
         * This method binds the Texture to the current OpenGL context. This
         * allows the Texture to be used in subsequent OpenGL calls.
         *
         * @param slot The texture slot to bind the Texture to.
         */
        void bind(unsigned int slot = 0) const;

        /**
         * @brief Unbinds the Texture from the current OpenGL context.
         *
         * This method unbinds the Texture from the current OpenGL context. This
         * prevents the Texture from being used in subsequent OpenGL calls.
         */
        void unbind() const;

        /**
         * @brief Gets the width of the Texture.
         *
         * This method returns the width of the Texture.
         *
         * @return The width of the Texture.
         */
        inline int getWidth() const { return width; }

        /**
         * @brief Gets the height of the Texture.
         *
         * This method returns the height of the Texture.
         *
         * @return The height of the Texture.
         */
        inline int getHeight() const { return height; }

    private:
        // The OpenGL ID of the texture
        unsigned int id;
        // The path to the image file
        std::string filepath;
        // The image data
        unsigned char* localBuffer;
        // The width of the image
        int width;
        // The height of the image
        int height;
        // The number of bytes per pixel
        int bpp;
    };
}  // namespace Engine