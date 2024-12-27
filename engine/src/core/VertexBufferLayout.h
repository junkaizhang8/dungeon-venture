#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "utils/debug.h"

/**
 * @brief A struct representing a vertex buffer element.
 *
 * This struct represents a single element in a vertex buffer layout
 * corresponding to a vertex attribute. Each element has a type indicating the
 * OpenGL type of the vertex attribute, a count indicating the number of
 * elements in the vertex attribute, and a flag indicating whether the data
 * should be normalized.
 */
struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    /**
     * @brief Returns the size of the specified OpenGL type.
     *
     * @param type
     * @return The size of the specified OpenGL type.
     */
    static unsigned int getSizeOfType(unsigned int type)
    {
        switch (type)
        {
            case GL_FLOAT:
                return 4;
            case GL_UNSIGNED_INT:
                return 4;
            case GL_UNSIGNED_BYTE:
                return 1;
        }
        ASSERT(false);
        return 0;
    }
};

/**
 * @brief A class representing a vertex buffer layout.
 *
 * This class represents the layout of a vertex buffer. It is used to specify
 * the layout of the data in the vertex buffer, indicating the different vertex
 * attributes through pushing elements of different types.
 */
class VertexBufferLayout
{
public:
    /**
     * @brief Creates a new VertexBufferLayout object.
     *
     * This constructor creates a new VertexBufferLayout object with an empty
     * layout.
     */
    VertexBufferLayout() : stride(0) {}

    /**
     * @brief Pushes a new vertex attribute of the specified type and count to
     * the layout.
     *
     * This method pushes a new vertex attribute of the specified type and count
     * to the layout.
     *
     * @tparam T The type of the vertex attribute.
     * @param count The number of elements in the vertex attribute.
     */
    template <typename T>
    void push(unsigned int count)
    {
        static_assert(false);
    }

    /**
     * @brief Pushes a new vertex attribute of type float to the layout.
     *
     * This method pushes a new vertex attribute of type float to the layout.
     *
     * @param count The number of elements in the vertex attribute.
     */
    template <>
    void push<float>(unsigned int count)
    {
        elements.push_back({GL_FLOAT, count, GL_FALSE});
        stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
    }

    /**
     * @brief Pushes a new vertex attribute of type unsigned int to the layout.
     *
     * This method pushes a new vertex attribute of type unsigned int to the
     * layout.
     *
     * @param count The number of elements in the vertex attribute.
     */
    template <>
    void push<unsigned int>(unsigned int count)
    {
        elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
        stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
    }

    /**
     * @brief Pushes a new vertex attribute of type unsigned char to the layout.
     *
     * @param count The number of elements in the vertex attribute.
     */
    template <>
    void push<unsigned char>(unsigned int count)
    {
        elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
        stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
    }

    /**
     * @brief Returns the elements in the layout.
     *
     * @return A vector containing the elements in the layout.
     */
    inline const std::vector<VertexBufferElement>& getElements() const
    {
        return elements;
    }

    /**
     * @brief Returns the stride of the layout.
     *
     * @return The stride of the layout.
     */
    inline unsigned int getStride() const { return stride; }

private:
    // The elements in the layout
    std::vector<VertexBufferElement> elements;
    // The stride of the layout
    unsigned int stride;
};