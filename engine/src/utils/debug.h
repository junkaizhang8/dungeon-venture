#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#define ASSERT(condition)                                             \
    if (!(condition))                                                 \
    {                                                                 \
        std::cerr << "[Assertion Failure] (" << #condition << ") at " \
                  << __FILE__ << ":" << __LINE__ << std::endl;        \
        std::exit(EXIT_FAILURE);                                      \
    }

#define glCall(x)   \
    glClearError(); \
    x;              \
    glLogCall(#x, __FILE__, __LINE__)

static void glClearError() { while (glGetError() != GL_NO_ERROR); }

static void glLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cerr << "[OpenGL Error] (" << error << "): " << function << " "
                  << file << ":" << line << std::endl;
        std::exit(EXIT_FAILURE);
    }
}