#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace Engine
{
#ifdef ENGINE_DEBUG
#ifdef _MSC_VER
#define DEBUG_BREAK() __debugbreak()
#elif defined(__clang__) || defined(__GNUC__)
#define DEBUG_BREAK() __builtin_trap()
#else
#include <csignal>
#define DEBUG_BREAK() std::raise(SIGTRAP)
#endif
#else
#define DEBUG_BREAK() std::exit(EXIT_FAILURE)
#endif

#define TERMINAL_RESET "\033[0m"
#define TERMINAL_BOLD "\033[1m"
#define TERMINAL_RED_BOLD "\033[1;31m"
#define TERMINAL_MAGENTA_BOLD "\033[1;35m"
#define TERMINAL_BLACK_BOLD "\033[1;30m"

#define LOG_ERROR(...)                                                   \
    {                                                                    \
        fprintf(stderr,                                                  \
                TERMINAL_BOLD "%s:%d: " TERMINAL_RESET TERMINAL_RED_BOLD \
                              "[Error]: " TERMINAL_RESET,                \
                __FILE__, __LINE__);                                     \
        fprintf(stderr, __VA_ARGS__);                                    \
        fprintf(stderr, "\n");                                           \
        DEBUG_BREAK();                                                   \
    }

#define LOG_WARN(...)                                                        \
    {                                                                        \
        fprintf(stderr,                                                      \
                TERMINAL_BOLD "%s:%d: " TERMINAL_RESET TERMINAL_MAGENTA_BOLD \
                              "[Warning]: " TERMINAL_RESET,                  \
                __FILE__, __LINE__);                                         \
        fprintf(stderr, __VA_ARGS__);                                        \
        fprintf(stderr, "\n");                                               \
    }

#define LOG_INFO(...)                                                      \
    {                                                                      \
        fprintf(stdout,                                                    \
                TERMINAL_BOLD "%s:%d: " TERMINAL_RESET TERMINAL_BLACK_BOLD \
                              "[Info]: " TERMINAL_RESET,                   \
                __FILE__, __LINE__);                                       \
        fprintf(stdout, __VA_ARGS__);                                      \
        fprintf(stdout, "\n");                                             \
    }

#define GL_LOG_ERROR(...)                                                \
    {                                                                    \
        fprintf(stderr,                                                  \
                TERMINAL_BOLD "%s:%d: " TERMINAL_RESET TERMINAL_RED_BOLD \
                              "[OpenGL Error]: " TERMINAL_RESET,         \
                __FILE__, __LINE__);                                     \
        fprintf(stderr, __VA_ARGS__);                                    \
        fprintf(stderr, "\n");                                           \
        DEBUG_BREAK();                                                   \
    }

#define GL_LOG_WARN(...)                                                     \
    {                                                                        \
        fprintf(stderr,                                                      \
                TERMINAL_BOLD "%s:%d: " TERMINAL_RESET TERMINAL_MAGENTA_BOLD \
                              "[OpenGL Warning]: " TERMINAL_RESET,           \
                __FILE__, __LINE__);                                         \
        fprintf(stderr, __VA_ARGS__);                                        \
        fprintf(stderr, "\n");                                               \
    }

#define GL_LOG_INFO(...)                                                   \
    {                                                                      \
        fprintf(stdout,                                                    \
                TERMINAL_BOLD "%s:%d: " TERMINAL_RESET TERMINAL_BLACK_BOLD \
                              "[OpenGL Info]: " TERMINAL_RESET,            \
                __FILE__, __LINE__);                                       \
        fprintf(stdout, __VA_ARGS__);                                      \
        fprintf(stdout, "\n");                                             \
    }

#define ASSERT(condition, ...)                           \
    if (!(condition))                                    \
    {                                                    \
        LOG_ERROR("Assertion failed: (%s)", #condition); \
    }
}  // namespace Engine