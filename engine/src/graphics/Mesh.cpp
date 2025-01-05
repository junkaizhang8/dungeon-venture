#include "Mesh.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Engine
{
    Mesh::Mesh(const std::string& name, const std::vector<Vertex>& vertices,
               const std::vector<unsigned int>& indices, MeshType type,
               DrawMode mode,
               const std::optional<CustomAttributeLayout>& customLayout)
        : name(name),
          vertices(vertices),
          indices(indices),
          type(type),
          mode(mode),
          customLayout(customLayout.value_or(CustomAttributeLayout()))
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

        // Unbind the vertex array
        glBindVertexArray(0);
        // Unbind the vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Unbind the index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // Create the custom attribute buffer objects if a custom layout is
        // provided
        if (customLayout)
        {
            const auto& layout = customLayout.value();
            if (layout.getElements().size() > 0)
            {
                customvbos.resize(layout.getElements().size());
                hasCustomLayout = true;
            }
            else
                hasCustomLayout = false;
        }
        else
            hasCustomLayout = false;
    }

    Mesh::~Mesh()
    {
        // Delete the vertex array and buffers
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);

        // Delete the custom attribute buffers
        for (unsigned int customvbo : customvbos)
            if (customvbo) glDeleteBuffers(1, &customvbo);
    }

    void Mesh::draw(const Shader& shader)
    {
        glBindVertexArray(vao);
        glDrawElements(static_cast<GLenum>(type), indices.size(),
                       GL_UNSIGNED_INT, nullptr);
    }
}  // namespace Engine