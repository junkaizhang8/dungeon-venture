#pragma once

#include <glm/glm.hpp>

namespace Engine
{
    /**
     * @brief A struct that represents a vertex.
     *
     * This struct represents a vertex in a mesh. A vertex may contain the
     * following attributes, where [ ] denotes the location of the attribute in
     * the shader and ( ) denotes the type of the attribute:
     *
     * - [0] (vec3) position: The xyz values of the vertex.
     *
     * - [1] (vec4) color: The rgba values of the vertex.
     *
     * - [2] (vec2) texCoords: The texture coordinates of the vertex.
     */
    struct Vertex
    {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 texCoords;

        /**
         * @brief Gets the number of attributes in the vertex.
         *
         * This method returns the number of attributes in the vertex.
         *
         * @return The number of attributes in the vertex.
         */
        static unsigned int getAttributeCount() { return 3; }
    };
}  // namespace Engine