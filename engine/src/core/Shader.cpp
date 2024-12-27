#include "Shader.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>

#include "utils/debug.h"

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

// Convert a shader type to an OpenGL enum
static unsigned int castShaderType(Shader::Type type)
{
    switch (type)
    {
        case Shader::Type::VERTEX:
            return GL_VERTEX_SHADER;
        case Shader::Type::FRAGMENT:
            return GL_FRAGMENT_SHADER;
        case Shader::Type::GEOMETRY:
            return GL_GEOMETRY_SHADER;
        case Shader::Type::COMPUTE:
            return GL_COMPUTE_SHADER;
    }
    ASSERT(false);
    return 0;
}

Shader::~Shader() { glCall(glDeleteProgram(id)); }

void Shader::bind() const { glCall(glUseProgram(id)); }

void Shader::unbind() const { glCall(glUseProgram(0)); }

void Shader::addShader(Type type, const std::string& path)
{
    unsigned int shader = 0;
    // Extract the shader source from the file
    std::string code = readFile(path);
    const char* src = code.c_str();

    glCall(shader = glCreateShader(castShaderType(type)));
    glCall(glShaderSource(shader, 1, &src, nullptr));
    glCall(glCompileShader(shader));

    // Add the shader to the list (will overwrite any existing shader of the
    // same type)
    shaders[type] = shader;
}

void Shader::compileShader()
{
    // Create the shader program
    glCall(id = glCreateProgram());

    // Attach the shaders to the program
    for (const auto& shader : shaders)
    {
        glCall(glAttachShader(id, shader.second));
    }

    // Link the program
    glCall(glLinkProgram(id));

    // Clean up the shaders (not needed after linking)
    for (const auto& shader : shaders)
    {
        glCall(glDeleteShader(shader.second));
    }
}

void Shader::setUniform1i(const std::string& name, int value) const
{
    glCall(glUniform1i(getUniformLocation(name), value));
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2,
                          float v3) const
{
    glCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setUniformMat4f(const std::string& name,
                             const glm::mat4& matrix) const
{
    glCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE,
                              &matrix[0][0]));
}

int Shader::getUniformLocation(const std::string& name) const
{
    // Check if the uniform location has already been cached
    if (uniformLocations.find(name) != uniformLocations.end())
        return uniformLocations[name];

    // Get the uniform location
    glCall(int location = glGetUniformLocation(id, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' not found in shader"
                  << std::endl;
    // Cache the uniform location
    uniformLocations[name] = location;
    return location;
}