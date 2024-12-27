#include "IndexBuffer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "utils/debug.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : count(count)
{
    glCall(glGenBuffers(1, &id));
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
    glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int),
                        data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &id); }

void IndexBuffer::bind() const
{
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
}

void IndexBuffer::unbind() const
{
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}