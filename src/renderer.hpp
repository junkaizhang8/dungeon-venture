#pragma once

#include <GLFW/glfw3.h>

#define RED 0xff0000
#define GREEN 0x00ff00
#define BLUE 0x0000ff
#define YELLOW 0xffff00
#define BLACK 0x000000
#define WHITE 0xffffff
#define CIRCLE_SEGMENT_NUM 100

typedef int rgb_hex;

class Renderer
{
public:
    Renderer() = default;
    ~Renderer() = default;
    void init(double left, double right, double bottom, double top, int scale);
    void clearScreen();
    void drawPixel(double x, double y, rgb_hex rgb = BLACK);
    void drawQuad(double vertices[4][2], rgb_hex rgb = BLACK);
    void drawRect(double x, double y, double width, double height, rgb_hex rgb = BLACK);
    void drawFilledCircle(double x, double y, double r, rgb_hex = BLACK);
    void drawHollowCircle(double x, double y, double r, rgb_hex = BLACK);

private:
    int pixelScale;
    int pixelScaleHalf;
    void setColour(rgb_hex rgb);
};