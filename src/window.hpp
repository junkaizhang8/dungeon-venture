#pragma once

#include <iostream>
#include <memory>
#include <GLFW/glfw3.h>
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
    virtual int init(int width, int height, int pixelScale, const char *title) = 0;
    virtual void display() = 0;
    virtual bool isRunning() = 0;
    virtual void close() = 0;
    
protected:
    GLFWwindow *window = nullptr;
    std::shared_ptr<Renderer> renderer;
    Clock clock;
    int windowWidth;
    int windowHeight;
};