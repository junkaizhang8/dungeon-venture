#pragma once

/**
 * @brief A class that encapsulates an OpenGL index buffer object.
 *
 * This class provides a simple interface for creating and managing index buffer
 * objects in OpenGL.
 */
class IndexBuffer
{
public:
    /**
     * @brief Creates a new IndexBuffer object.
     *
     * This constructor creates a new IndexBuffer object and initializes it with
     * the specified data.
     *
     * @param data A pointer to the data to be stored in the index buffer.
     * @param count The number of indices in the index buffer.
     */
    IndexBuffer(const unsigned int* data, unsigned int count);

    /**
     * @brief Destroys the IndexBuffer object.
     *
     * This destructor destroys the IndexBuffer object and frees any resources
     * associated with it.
     */
    ~IndexBuffer();

    /**
     * @brief Binds the IndexBuffer to the current OpenGL context.
     *
     * This method binds the IndexBuffer to the current OpenGL context. This
     * allows the IndexBuffer to be used in subsequent OpenGL calls.
     */
    void bind() const;

    /**
     * @brief Unbinds the IndexBuffer from the current OpenGL context.
     *
     * This method unbinds the IndexBuffer from the current OpenGL context. This
     * prevents the IndexBuffer from being used in subsequent OpenGL calls.
     */
    void unbind() const;

    /**
     * @brief Returns the number of indices in the IndexBuffer.
     *
     * This method returns the number of indices in the IndexBuffer.
     *
     * @return The number of indices in the IndexBuffer.
     */
    inline unsigned int getCount() const { return count; }

private:
    // The OpenGL ID of the index buffer object.
    unsigned int id;
    // The number of indices in the index buffer.
    unsigned int count;
};