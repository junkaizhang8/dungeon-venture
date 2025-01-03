#include "Mesh.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Engine
{
    Mesh::Mesh(const std::string& name, const std::vector<Vertex>& vertices,
               const std::vector<unsigned int>& indices, MeshType type,
               DrawMode mode,
               const std::optional<CustomBufferLayoutPair>& custom)
        : name(name),
          vertices(vertices),
          indices(indices),
          type(type),
          mode(mode)
    {
        // Create the vertex array
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ibo);

        // Bind the vertex array
        glBindVertexArray(vao);

        // Bind the vertex buffer and load the vertex data
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                     &vertices[0], static_cast<GLenum>(mode));

        // Bind the index buffer and load the index data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     indices.size() * sizeof(unsigned int), &indices[0],
                     static_cast<GLenum>(mode));

        // Set the vertex attribute pointers
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void*)offsetof(Vertex, color));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void*)offsetof(Vertex, texCoords));

        // If there are custom attributes, set the vertex attribute pointers
        if (custom)
        {
            const auto& buffer = custom->buffer;
            const auto& layout = custom->layout;
            unsigned int offset = 0;
            const auto& elements = layout.getElements();

            // Create the custom buffer object
            glGenBuffers(1, &customvbo);
            // Bind the custom buffer object and load the custom data
            glBindBuffer(GL_ARRAY_BUFFER, customvbo);
            glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float),
                         &buffer[0], static_cast<GLenum>(mode));

            hasCustom = true;

            // Set the custom attribute pointers
            for (unsigned int i = 0; i < elements.size(); ++i)
            {
                const auto& element = elements[i];
                glEnableVertexAttribArray(i + 3);
                glVertexAttribPointer(
                    i + 3, element.count, element.type, element.normalized,
                    layout.getStride(),
                    reinterpret_cast<const void*>(std::uintptr_t(offset)));
                offset += element.count * sizeof(float);
            }
        }

        // Unbind the vertex array
        glBindVertexArray(0);
        // Unbind the vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Unbind the index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    Mesh::~Mesh()
    {
        // Delete the vertex array and buffers
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);

        // Delete the custom buffer object if it exists
        if (hasCustom)
        {
            glDeleteBuffers(1, &customvbo);
        }
    }

    void Mesh::draw(const Shader& shader)
    {
        glBindVertexArray(vao);
        glDrawElements(static_cast<GLenum>(type), indices.size(),
                       GL_UNSIGNED_INT, nullptr);
    }
}  // namespace Engine