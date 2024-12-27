#pragma once

/**
 * @brief A class that encapsulates an OpenGL vertex buffer object.
 *
 * This class provides a simple interface for creating and managing vertex
 * buffer objects in OpenGL.
 */
class VertexBuffer
{
public:
    /**
     * @brief Creates a new VertexBuffer object.
     *
     * This constructor creates a new VertexBuffer object and initializes it
     * with the specified data.
     *
     * @param data A pointer to the data to be stored in the vertex buffer.
     * @param size The size of the data in bytes.
     */
    VertexBuffer(const void* data, unsigned int size);

    /**
     * @brief Destroys the VertexBuffer object.
     *
     * This destructor destroys the VertexBuffer object and frees any resources
     * associated with it.
     */
    ~VertexBuffer();

    /**
     * @brief Binds the VertexBuffer to the current OpenGL context.
     *
     * This method binds the VertexBuffer to the current OpenGL context. This
     * allows the VertexBuffer to be used in subsequent OpenGL calls.
     */
    void bind() const;

    /**
     * @brief Unbinds the VertexBuffer from the current OpenGL context.
     *
     * This method unbinds the VertexBuffer from the current OpenGL context.
     * This prevents the VertexBuffer from being used in subsequent OpenGL
     * calls.
     */
    void unbind() const;

private:
    // The OpenGL ID of the vertex buffer object.
    unsigned int id;
};