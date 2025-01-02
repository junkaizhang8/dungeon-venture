#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <optional>
#include <string>

#include "Shader.h"

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
    };

    /**
     * @brief An enum class that represents the type of mesh.
     *
     * This enum class represents the type of mesh that is being drawn.
     */
    enum class MeshType
    {
        TRIANGLES = GL_TRIANGLES,
        LINES = GL_LINES
    };

    /**
     * @brief An enum class that represents the draw mode of the mesh.
     *
     * This enum class represents the draw mode of the mesh.
     */
    enum class DrawMode
    {
        STATIC = GL_STATIC_DRAW,
        DYNAMIC = GL_DYNAMIC_DRAW
    };

    /**
     * @brief A class that represents a mesh.
     *
     * This class represents a mesh that can be drawn in the scene. A mesh is
     * composed of vertices and indices that define the shape of the mesh.
     */
    class Mesh
    {
    public:
        /**
         * @brief Constructs a new Mesh object.
         *
         * This constructor creates a new Mesh object with the specified name,
         * vertices, and indices. The type of the mesh is set to TRIANGLES by
         * default, and the draw mode is set to STATIC by default.
         *
         * @param name The name of the mesh.
         * @param vertices The vertices of the mesh.
         * @param indices The indices of the mesh.
         * @param type The type of the mesh. Defaults to MeshType::TRIANGLES.
         * @param mode The draw mode of the mesh. Defaults to DrawMode::STATIC.
         */
        Mesh(const std::string& name, std::vector<Vertex>& vertices,
             std::vector<unsigned int>& indices,
             MeshType type = MeshType::TRIANGLES,
             DrawMode mode = DrawMode::STATIC);

        /**
         * @brief Destroys the Mesh object.
         *
         * This destructor destroys the Mesh object and frees any resources
         * associated with it.
         */
        ~Mesh();

        /**
         * @brief Draws the mesh.
         *
         * This method draws the mesh using the specified shader.
         *
         * @param shader The shader to use when drawing the mesh.
         */
        void draw(const Shader& shader);

        /**
         * @brief Gets the name of the mesh.
         *
         * This method returns the name of the mesh.
         *
         * @return The name of the mesh.
         */
        inline const std::string& getName() const { return name; }

    private:
        // The name of the mesh
        std::string name;
        // The vertices of the mesh
        std::vector<Vertex> vertices;
        // The indices of the mesh
        std::vector<unsigned int> indices;

        // The type of the mesh
        MeshType type;
        // The draw mode of the mesh
        DrawMode mode;

        // The vertex array object
        unsigned int vao;
        // The vertex buffer object
        unsigned int vbo;
        // The index buffer object
        unsigned int ibo;
    };
}  // namespace Engine