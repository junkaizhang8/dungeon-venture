#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

/**
 * @brief A class that encapsulates an OpenGL vertex array object.
 *
 * This class provides a simple interface for creating and managing vertex array
 * objects in OpenGL. The class is designed to be used in conjunction with the
 * VertexBuffer class and the VertexBufferLayout class.
 */
class VertexArray
{
public:
    /**
     * @brief Creates a new VertexArray object.
     *
     * This constructor creates a new VertexArray object and initializes it with
     * an OpenGL vertex array object.
     */
    VertexArray();

    /**
     * @brief Destroys the VertexArray object.
     *
     * This destructor destroys the VertexArray object and frees any resources
     * associated with it.
     */
    ~VertexArray();

    /**
     * @brief Binds the VertexArray to the current OpenGL context.
     *
     * This method binds the VertexArray to the current OpenGL context. This
     * allows the VertexArray to be used in subsequent OpenGL calls.
     */
    void bind() const;

    /**
     * @brief Unbinds the VertexArray from the current OpenGL context.
     *
     * This method unbinds the VertexArray from the current OpenGL context. This
     * prevents the VertexArray from being used in subsequent OpenGL calls.
     */
    void unbind() const;

    /**
     * @brief Adds a vertex buffer to the VertexArray with the specified layout.
     *
     * This method adds a vertex buffer to the VertexArray with the specified
     * layout. The layout specifies the layout of the data in the vertex buffer.
     *
     * @param vb The vertex buffer to add to the VertexArray.
     * @param layout The layout of the data in the vertex buffer.
     */
    void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

private:
    // The OpenGL ID of the vertex array object.
    unsigned int id;
};