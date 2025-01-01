#include "IndexBuffer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Engine
{
    IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
        : count(count)
    {
        glGenBuffers(1, &id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int),
                     data, GL_STATIC_DRAW);
    }

    IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &id); }

    void IndexBuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    }

    void IndexBuffer::unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}  // namespace Engine