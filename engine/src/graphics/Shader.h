#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace Engine
{
    /**
     * @brief An enum class that represents the type of shader.
     *
     * This enum class represents the type of shader that is being created.
     */
    enum class ShaderType
    {
        VERTEX = GL_VERTEX_SHADER,
        FRAGMENT = GL_FRAGMENT_SHADER,
        GEOMETRY = GL_GEOMETRY_SHADER
    };

    /**
     * @brief A class that encapsulates an OpenGL shader object.
     *
     * This class provides a simple interface for creating and managing shader
     * programs in OpenGL.
     */
    class Shader
    {
    public:
        /**
         * @brief Creates a new Shader object.
         *
         * This constructor creates a new Shader object and initializes it with
         * an empty shader program.
         */
        Shader() = default;

        /**
         * @brief Destroys the Shader object.
         *
         * This destructor destroys the Shader object and frees any resources
         * associated with it.
         */
        ~Shader();

        /**
         * @brief Binds the Shader to the current OpenGL context.
         *
         * This method binds the Shader to the current OpenGL context. This
         * allows the Shader to be used in subsequent OpenGL calls.
         */
        void bind() const;

        /**
         * @brief Unbinds the Shader from the current OpenGL context.
         *
         * This method unbinds the Shader from the current OpenGL context. This
         * prevents the Shader from being used in subsequent OpenGL calls.
         */
        void unbind() const;

        /**
         * @brief Adds a shader of the specified type to the Shader.
         *
         * This method adds a shader of the specified type to the Shader. The
         * shader is loaded from the file at the specified path.
         *
         * @param type The type of the shader to add.
         * @param path The path to the file containing the shader source.
         */
        void addShader(ShaderType type, const std::string& path);

        /**
         * @brief Compiles the Shader program.
         *
         * This method compiles the Shader program by attaching the shaders to
         * the program and linking the program.
         */
        void compileShader();

        /**
         * @brief Sets the Uniform1i object.
         *
         * @param name The name of the uniform.
         * @param v0 The value.
         */
        void setUniform1i(const std::string& name, int v0) const;

        /**
         * @brief Sets the Uniform4f object.
         *
         * @param name The name of the uniform.
         * @param v0 The first value.
         * @param v1 The second value.
         * @param v2 The third value.
         * @param v3 The fourth value.
         */
        void setUniform4f(const std::string& name, float v0, float v1, float v2,
                          float v3) const;

        /**
         * @brief Sets the Uniform Mat4f object.
         *
         * @param name The name of the uniform.
         * @param matrix The matrix.
         */
        void setUniformMat4f(const std::string& name,
                             const glm::mat4& matrix) const;

    private:
        // The OpenGL ID of the shader program.
        unsigned int id;
        // An unordered map of shader types to shader IDs.
        mutable std::unordered_map<ShaderType, unsigned int> shaders;
        // An unordered map of uniform names to uniform locations.
        mutable std::unordered_map<std::string, int> uniformLocations;

        /**
         * @brief Returns the location of the specified uniform.
         *
         * @param name The name of the uniform
         * @return The location of the uniform
         */
        int getUniformLocation(const std::string& name) const;
    };
}  // namespace Engine