#include "VertexBuffer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "utils/debug.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    glCall(glGenBuffers(1, &id));
    glCall(glBindBuffer(GL_ARRAY_BUFFER, id));
    glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() { glCall(glDeleteBuffers(1, &id)); }

void VertexBuffer::bind() const { glCall(glBindBuffer(GL_ARRAY_BUFFER, id)); }

void VertexBuffer::unbind() const { glCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }