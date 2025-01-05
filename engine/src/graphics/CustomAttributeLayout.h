#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "Vertex.h"

namespace Engine
{
    /**
     * @brief An enum class that represents the type of attribute.
     *
     * This enum class represents the type of attribute in a custom vertex
     * buffer layout.
     */
    enum class AttributeType
    {
        FLOAT = GL_FLOAT,
        INT = GL_INT,
        UNSIGNED_INT = GL_UNSIGNED_INT,
        UNSIGNED_BYTE = GL_UNSIGNED_BYTE
    };

    /**
     * @brief A struct representing a custom attribute.
     *
     * This struct represents a single custom attribute in a custom vertex
     * attribute layout. Each element has a type indicating the OpenGL type
     * of the vertex attribute, a count indicating the number of elements in
     * the vertex attribute, and a flag indicating whether the data should
     * be normalized.
     */
    struct CustomAttribute
    {
        AttributeType type;
        unsigned int count;
        unsigned char normalized;

        /**
         * @brief Constructs a new CustomAttribute object.
         *
         * This constructor creates a new CustomAttribute object with the
         * specified type, count, and normalized flag.
         *
         * @param type The type of the attribute.
         * @param count The number of elements in the attribute.
         * @param normalized A flag indicating whether the data should be
         * normalized.
         */
        CustomAttribute(AttributeType type, unsigned int count,
                        unsigned char normalized)
            : type(type), count(count), normalized(normalized)
        {
        }

        /**
         * @brief Returns the size of the specified attribute type.
         *
         * @param type The type of the attribute.
         * @return The size of the specified attribute type.
         */
        static unsigned int getSizeOfType(AttributeType type)
        {
            switch (type)
            {
                case AttributeType::FLOAT:
                    return sizeof(float);
                case AttributeType::INT:
                    return sizeof(int);
                case AttributeType::UNSIGNED_INT:
                    return sizeof(unsigned int);
                case AttributeType::UNSIGNED_BYTE:
                    return sizeof(unsigned char);
                default:
                    return 0;
            }
        }
    };

    /**
     * @brief A class representing a custom vertex buffer layout.
     *
     * This class represents the layout of a vertex buffer. It is used to
     * specify the layout of the data in a custom vertex buffer, indicating the
     * different vertex attributes through pushing elements of different types
     * to the layout. If used in conjunction with a mesh, the layout locations
     * of the custom attributes will start at 3, as the first three locations
     * are reserved for the standard vertex attributes.
     */
    class CustomAttributeLayout
    {
    public:
        /**
         * @brief Creates a new CustomAttributeLayout object.
         *
         * This constructor creates a new CustomAttributeLayout object with an
         * empty layout.
         */
        CustomAttributeLayout() : stride(0) {}

        /**
         * @brief Adds a new attribute to the layout.
         *
         * This method adds a new attribute to the layout with the specified
         * type and count. The method returns the index of the attribute in the
         * layout.
         *
         * @param type The type of the attribute.
         * @param count The number of elements in the attribute.
         * @return The index of the attribute in the layout.
         */
        int addAttribute(AttributeType type, unsigned int count);

        /**
         * @brief Gets the element at the specified index.
         *
         * This method gets the element at the specified index in the layout.
         *
         * @param index The index of the element.
         * @return The element at the specified index, or nullptr if the index
         * is out of bounds.
         */
        const CustomAttribute* getElement(int index) const;

        /**
         * @brief Gets the elements in the layout.
         *
         * @return A vector containing the elements in the layout.
         */
        inline const std::vector<CustomAttribute>& getElements() const
        {
            return elements;
        }

        /**
         * @brief Gets the stride of the layout.
         *
         * @return The stride of the layout.
         */
        inline unsigned int getStride() const { return stride; }

    private:
        // The elements in the layout
        std::vector<CustomAttribute> elements;
        // The stride of the layout
        unsigned int stride;
    };
}  // namespace Engine