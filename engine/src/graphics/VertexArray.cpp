#include "VertexArray.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Engine
{
    VertexArray::VertexArray() { glGenVertexArrays(1, &id); }

    VertexArray::~VertexArray() { glDeleteVertexArrays(1, &id); }

    void VertexArray::bind() const { glBindVertexArray(id); }

    void VertexArray::unbind() const { glBindVertexArray(0); }

    void VertexArray::addBuffer(const VertexBuffer& vb,
                                const VertexBufferLayout& layout)
    {
        // Bind the vertex array
        bind();
        // Bind the vertex buffer
        vb.bind();
        const auto& elements = layout.getElements();
        unsigned int offset = 0;
        // Set the layout of the vertex buffer
        for (unsigned int i = 0; i < elements.size(); ++i)
        {
            const auto& element = elements[i];
            // Enable the vertex attribute
            glEnableVertexAttribArray(i);
            // Configure the vertex attribute
            glVertexAttribPointer(
                i, element.count, element.type, element.normalized,
                layout.getStride(),
                reinterpret_cast<const void*>(std::uintptr_t(offset)));
            offset += element.count *
                      VertexBufferElement::getSizeOfType(element.type);
        }
    }
}  // namespace Engine