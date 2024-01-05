#pragma once

#include <GLFW/glfw3.h>

class Renderer
{
public:
    Renderer() = default;
    ~Renderer() = default;
    void init(double left, double right, double bottom, double top, int scale);
    void setColour(int red, int green, int blue);
    void setColour(float red, float green, float blue);
    void clearScreen();
    void drawPixel(double x, double y);
    void drawRect(double x, double y, double width, double height);
    void drawFilledCircle(double x, double y, double r, int numSegments = 100);
    void drawHollowCircle(double x, double y, double r, int numSegments = 100);

private:
    int pixelScale;
    int pixelScaleHalf;
};