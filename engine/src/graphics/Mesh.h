#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <optional>
#include <string>

#include "CustomAttributeLayout.h"
#include "Shader.h"
#include "Vertex.h"
#include "utils/EngineDebug.h"

namespace Engine
{
    /**
     * @brief An enum class that represents the type of mesh.
     *
     * This enum class represents the type of mesh that is being drawn.
     */
    enum class MeshType
    {
        POINTS = GL_POINTS,
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
         * vertices, indices, and optionally a custom buffer-layout pair for
         * custom attributes. The type of the mesh is set to TRIANGLES by
         * default, and the draw mode is set to STATIC by default.
         *
         * @param name The name of the mesh.
         * @param vertices The vertices of the mesh.
         * @param indices The indices of the mesh.
         * @param type The type of the mesh. Defaults to MeshType::TRIANGLES.
         * @param mode The draw mode of the mesh. Defaults to DrawMode::STATIC.
         * @param customLayout An optional custom attribute layout for the mesh.
         * Defaults to std::nullopt.
         */
        Mesh(const std::string& name, const std::vector<Vertex>& vertices,
             const std::vector<unsigned int>& indices,
             MeshType type = MeshType::TRIANGLES,
             DrawMode mode = DrawMode::STATIC,
             const std::optional<CustomAttributeLayout>& customLayout =
                 std::nullopt);

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
         * @brief Attaches a custom buffer to the mesh.
         *
         * This method attaches a custom buffer to the mesh at the specified
         * index. The buffer must be a vector of the specified type. The method
         * will generate a buffer object and bind the buffer to the mesh.
         *
         * @tparam T The type of the buffer.
         * @param index The index of the custom attribute in the shader.
         * @param buffer The buffer to attach to the mesh.
         */
        template <typename T>
        void attachCustomBuffer(int index, const std::vector<T>& buffer)
        {
            static_assert(false);
        }

        /**
         * @brief Attaches a custom float buffer to the mesh.
         *
         * This method attaches a custom float buffer to the mesh at the
         * specified index. The buffer must be a vector of floats. The method
         * will generate a buffer object and bind the buffer to the mesh.
         *
         * @param index The index of the custom attribute in the shader.
         * @param buffer The buffer to attach to the mesh.
         */
        template <>
        void attachCustomBuffer<float>(int index,
                                       const std::vector<float>& buffer)
        {
            if (!hasCustomLayout)
            {
                LOG_WARN("Custom layout not found");
                return;
            }

            const CustomAttribute* attribute = customLayout.getElement(index);

            if (!attribute)
            {
                LOG_WARN("Custom attribute at index %d not found", index);
                return;
            }

            ASSERT(attribute->type == AttributeType::FLOAT,
                   "Buffer type must be float");

            ASSERT(buffer.size() / attribute->count == vertices.size(),
                   "Invalid buffer size");

            glGenBuffers(1,
                         &customvbos.at(index - Vertex::getAttributeCount()));
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER,
                         customvbos.at(index - Vertex::getAttributeCount()));
            glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float),
                         &buffer[0], static_cast<GLenum>(mode));
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, attribute->count, GL_FLOAT,
                                  attribute->normalized, 0, (void*)0);
            // Unbind the buffer
            glBindVertexArray(0);
            // Unbind the vertex buffer
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        /**
         * @brief Attaches a custom int buffer to the mesh.
         *
         * This method attaches a custom int buffer to the mesh at the specified
         * index. The buffer must be a vector of ints. The method will generate
         * a buffer object and bind the buffer to the mesh.
         *
         * @param index The index of the custom attribute in the shader.
         * @param buffer The buffer to attach to the mesh.
         */
        template <>
        void attachCustomBuffer<int>(int index, const std::vector<int>& buffer)
        {
            if (!hasCustomLayout)
            {
                LOG_WARN("Custom layout not found");
                return;
            }

            const CustomAttribute* attribute = customLayout.getElement(index);

            if (!attribute)
            {
                LOG_WARN("Custom attribute at index %d not found", index);
                return;
            }

            ASSERT(attribute->type == AttributeType::INT,
                   "Buffer type must be int");

            ASSERT(buffer.size() / attribute->count == vertices.size(),
                   "Invalid buffer size");

            glGenBuffers(1,
                         &customvbos.at(index - Vertex::getAttributeCount()));
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER,
                         customvbos.at(index - Vertex::getAttributeCount()));
            glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(int),
                         &buffer[0], static_cast<GLenum>(mode));
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, attribute->count, GL_INT,
                                  attribute->normalized, 0, (void*)0);
            // Unbind the buffer
            glBindVertexArray(0);
            // Unbind the vertex buffer
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        /**
         * @brief Attaches a custom unsigned int buffer to the mesh.
         *
         * This method attaches a custom unsigned int buffer to the mesh at the
         * specified index. The buffer must be a vector of unsigned ints. The
         * method will generate a buffer object and bind the buffer to the mesh.
         *
         * @param index The index of the custom attribute in the shader.
         * @param buffer The buffer to attach to the mesh.
         */
        template <>
        void attachCustomBuffer<unsigned int>(
            int index, const std::vector<unsigned int>& buffer)
        {
            if (!hasCustomLayout)
            {
                LOG_WARN("Custom layout not found");
                return;
            }

            const CustomAttribute* attribute = customLayout.getElement(index);

            if (!attribute)
            {
                LOG_WARN("Custom attribute at index %d not found", index);
                return;
            }

            ASSERT(attribute->type == AttributeType::UNSIGNED_INT,
                   "Buffer type must be unsigned int");

            ASSERT(buffer.size() / attribute->count == vertices.size(),
                   "Invalid buffer size");

            glGenBuffers(1,
                         &customvbos.at(index - Vertex::getAttributeCount()));
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER,
                         customvbos.at(index - Vertex::getAttributeCount()));
            glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(unsigned int),
                         &buffer[0], static_cast<GLenum>(mode));
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, attribute->count, GL_UNSIGNED_INT,
                                  attribute->normalized, 0, (void*)0);
            // Unbind the buffer
            glBindVertexArray(0);
            // Unbind the vertex buffer
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        /**
         * @brief Attaches a custom unsigned char buffer to the mesh.
         *
         * This method attaches a custom unsigned char buffer to the mesh at the
         * specified index. The buffer must be a vector of unsigned chars. The
         * method will generate a buffer object and bind the buffer to the mesh.
         *
         * @param index The index of the custom attribute in the shader.
         * @param buffer The buffer to attach to the mesh.
         */
        template <>
        void attachCustomBuffer<unsigned char>(
            int index, const std::vector<unsigned char>& buffer)
        {
            if (!hasCustomLayout)
            {
                LOG_WARN("Custom layout not found");
                return;
            }

            const CustomAttribute* attribute = customLayout.getElement(index);

            if (!attribute)
            {
                LOG_WARN("Custom attribute at index %d not found", index);
                return;
            }

            ASSERT(attribute->type == AttributeType::UNSIGNED_INT,
                   "Buffer type must be unsigned char");

            ASSERT(buffer.size() / attribute->count == vertices.size(),
                   "Invalid buffer size");

            glGenBuffers(1,
                         &customvbos.at(index - Vertex::getAttributeCount()));
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER,
                         customvbos.at(index - Vertex::getAttributeCount()));
            glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(unsigned char),
                         &buffer[0], static_cast<GLenum>(mode));
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, attribute->count, GL_UNSIGNED_BYTE,
                                  attribute->normalized, 0, (void*)0);
            // Unbind the buffer
            glBindVertexArray(0);
            // Unbind the vertex buffer
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

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
        // The custom attribute layout of the mesh
        CustomAttributeLayout customLayout;
        // Flag indicating whether the mesh has a custom layout
        bool hasCustomLayout;

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
        // A vector of custom vertex buffer objects
        std::vector<unsigned int> customvbos;
    };
}  // namespace Engine