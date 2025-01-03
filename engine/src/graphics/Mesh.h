#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <optional>
#include <string>

#include "Shader.h"
#include "utils/EngineDebug.h"

namespace Engine
{
    /**
     * @brief A struct that represents a vertex.
     *
     * This struct represents a vertex in a mesh. A vertex may contain the
     * following attributes, where [ ] denotes the location of the attribute in
     * the shader and ( ) denotes the type of the attribute:
     *
     * - [0] (vec3) position: The xyz values of the vertex.
     *
     * - [1] (vec4) color: The rgba values of the vertex.
     *
     * - [2] (vec2) texCoords: The texture coordinates of the vertex.
     */
    struct Vertex
    {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 texCoords;
    };

    /**
     * @brief An enum class that represents the type of mesh.
     *
     * This enum class represents the type of mesh that is being drawn.
     */
    enum class MeshType
    {
        POINTS = GL_POINTS,
        TRIANGLES = GL_TRIANGLES,
        LINES = GL_LINES
    };

    /**
     * @brief An enum class that represents the draw mode of the mesh.
     *
     * This enum class represents the draw mode of the mesh.
     */
    enum class DrawMode
    {
        STATIC = GL_STATIC_DRAW,
        DYNAMIC = GL_DYNAMIC_DRAW
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
            unsigned int type;
            unsigned int count;
            unsigned char normalized;
        };

        /**
         * @brief Creates a new CustomAttributeLayout object.
         *
         * This constructor creates a new CustomAttributeLayout object with an
         * empty layout.
         */
        CustomAttributeLayout() : stride(0) {}

        /**
         * @brief Pushes a new custom attribute of type float to the layout.
         *
         * This method pushes a new custom attribute of type float to the
         * layout.
         *
         * @param count The number of floats in the custom attribute.
         */
        void push(unsigned int count)
        {
            elements.push_back({GL_FLOAT, count, GL_FALSE});
            stride += count * sizeof(float);
        }

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

    /**
     * @brief A struct representing a custom buffer layout pair.
     *
     * This struct represents a pair of a custom buffer and a custom attribute
     * layout. It is used to define custom attributes for a mesh.
     */
    struct CustomBufferLayoutPair
    {
        std::vector<float> buffer;
        CustomAttributeLayout layout;

        /**
         * @brief Constructs a new CustomBufferLayoutPair object.
         *
         * This constructor creates a new CustomBufferLayoutPair object with the
         * specified buffer and layout.
         *
         * @param buffer The buffer containing the custom attributes.
         * @param layout The layout of the custom attributes.
         */
        CustomBufferLayoutPair(const std::vector<float>& buffer,
                               const CustomAttributeLayout& layout)
            : buffer(buffer), layout(layout)
        {
        }
    };

    /**
     * @brief A class that represents a mesh.
     *
     * This class represents a mesh that can be drawn in the scene. A mesh is
     * composed of vertices and indices that define the shape of the mesh.
     */
    class Mesh
    {
    public:
        /**
         * @brief Constructs a new Mesh object.
         *
         * This constructor creates a new Mesh object with the specified name,
         * vertices, indices, and optionally a custom buffer-layout pair for
         * custom attributes. The type of the mesh is set to TRIANGLES by
         * default, and the draw mode is set to STATIC by default.
         *
         * @param name The name of the mesh.
         * @param vertices The vertices of the mesh.
         * @param indices The indices of the mesh.
         * @param type The type of the mesh. Defaults to MeshType::TRIANGLES.
         * @param mode The draw mode of the mesh. Defaults to DrawMode::STATIC.
         * @param custom An optional buffer-layout pair for defining custom
         * attributes for the mesh. Defaults to std::nullopt.
         */
        Mesh(
            const std::string& name, const std::vector<Vertex>& vertices,
            const std::vector<unsigned int>& indices,
            MeshType type = MeshType::TRIANGLES,
            DrawMode mode = DrawMode::STATIC,
            const std::optional<CustomBufferLayoutPair>& custom = std::nullopt);

        /**
         * @brief Destroys the Mesh object.
         *
         * This destructor destroys the Mesh object and frees any resources
         * associated with it.
         */
        ~Mesh();

        /**
         * @brief Draws the mesh.
         *
         * This method draws the mesh using the specified shader.
         *
         * @param shader The shader to use when drawing the mesh.
         */
        void draw(const Shader& shader);

        /**
         * @brief Gets the name of the mesh.
         *
         * This method returns the name of the mesh.
         *
         * @return The name of the mesh.
         */
        inline const std::string& getName() const { return name; }

    private:
        // The name of the mesh
        std::string name;
        // The vertices of the mesh
        std::vector<Vertex> vertices;
        // The indices of the mesh
        std::vector<unsigned int> indices;

        // The type of the mesh
        MeshType type;
        // The draw mode of the mesh
        DrawMode mode;

        // The vertex array object
        unsigned int vao;
        // The vertex buffer object
        unsigned int vbo;
        // The index buffer object
        unsigned int ibo;
        // The custom attribute buffer object
        unsigned int customvbo;
        // Flag indicating if the mesh has custom attributes
        bool hasCustom = false;
    };
}  // namespace Engine