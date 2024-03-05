#pragma once

#include <GLFW/glfw3.h>
#include <memory>
#include "renderer.hpp"
#include "clock.hpp"

#define INIT_SUCCESS 0
#define INIT_ERROR -1
#define DEFAULT_PIXEL_SCALE 4

class Window
{
public:
    Window() = default;
    virtual ~Window() = default;
    void run();
    bool isRunning() { return !glfwWindowShouldClose(window); }
    void close();
    virtual int init(int width, int height, int pixelScale, const char *const title) = 0;
    virtual void update() = 0;
    virtual void display() = 0;
    
protected:
    GLFWwindow *window = nullptr;
    std::shared_ptr<Renderer> renderer;
    Clock clock;
    int windowWidth;
    int windowHeight;
};