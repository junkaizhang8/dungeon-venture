#pragma once

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Engine
{
    /**
     * @brief A class that encapsulates an OpenGL renderer.
     *
     * This class provides a simple interface for rendering objects using
     * OpenGL.
     */
    class Renderer
    {
    public:
        /**
         * @brief Draws the specified vertex array with the specified index
         * buffer and shader.
         *
         * This method draws the specified vertex array with the specified index
         * buffer and shader.
         *
         * @param va The vertex array to draw.
         * @param ib The index buffer to use for drawing.
         * @param shader The shader to use for drawing.
         */
        void draw(const VertexArray& va, const IndexBuffer& ib,
                  const Shader& shader) const;

        /**
         * @brief Clears the color buffer.
         *
         * This method clears the color buffer.
         */
        void clear() const;
    };
}  // namespace Engine