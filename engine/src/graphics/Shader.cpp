#include "Shader.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>

#include "utils/EngineDebug.h"

namespace Engine
{
    // Read the contents of a file
    static std::string readFile(const std::string& filePath)
    {
        std::ifstream stream(filePath);
        if (stream)
        {
            std::string content;
            stream.seekg(0, std::ios::end);
            content.resize(stream.tellg());
            stream.seekg(0, std::ios::beg);
            stream.read(&content[0], content.size());
            stream.close();
            return content;
        }
        throw std::runtime_error("Failed to open file: " + filePath);
    }

    Shader::~Shader() { glDeleteProgram(id); }

    void Shader::bind() const { glUseProgram(id); }

    void Shader::unbind() const { glUseProgram(0); }

    void Shader::addShader(ShaderType type, const std::string& path)
    {
        unsigned int shader = 0;
        // Extract the shader source from the file
        std::string code = readFile(path);
        const char* src = code.c_str();

        shader = glCreateShader(static_cast<GLenum>(type));
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        // Add the shader to the list (will overwrite any existing shader of the
        // same type)
        shaders[type] = shader;
    }

    void Shader::compileShader()
    {
        // Create the shader program
        id = glCreateProgram();

        // Attach the shaders to the program
        for (const auto& shader : shaders) glAttachShader(id, shader.second);

        // Link the program
        glLinkProgram(id);

        // Clean up the shaders (not needed after linking)
        for (const auto& shader : shaders) glDeleteShader(shader.second);
    }

    void Shader::setUniform1i(const std::string& name, int value) const
    {
        glUniform1i(getUniformLocation(name), value);
    }

    void Shader::setUniform4f(const std::string& name, float v0, float v1,
                              float v2, float v3) const
    {
        glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
    }

    void Shader::setUniformMat4f(const std::string& name,
                                 const glm::mat4& matrix) const
    {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE,
                           &matrix[0][0]);
    }

    int Shader::getUniformLocation(const std::string& name) const
    {
        // Check if the uniform location has already been cached
        if (uniformLocations.find(name) != uniformLocations.end())
            return uniformLocations[name];

        // Get the uniform location
        int location = glGetUniformLocation(id, name.c_str());
        if (location == -1) LOG_WARN("Uniform %s not found", name.c_str());
        // Cache the uniform location
        uniformLocations[name] = location;
        return location;
    }
}  // namespace Engine