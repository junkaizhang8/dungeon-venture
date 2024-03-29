#include <cmath>
#include "renderer.hpp"
#include <iostream>

void Renderer::init(double left, double right, double bottom, double top, int scale)
{
    pixelScale = scale;
    pixelScaleHalf = pixelScale / 2;

    glPointSize((float)pixelScale);
    glOrtho(left, right, bottom, top, -1.0, 1.0);
    glClearColor(0.2, 0.2, 0.2, 0);
}

void Renderer::clearScreen() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::drawPixel(double x, double y, rgb_hex rgb) const
{
    setColour(rgb);

    glBegin(GL_POINTS);
    glVertex2d(x * pixelScale, y * pixelScale);
    glEnd();
}

// Draw a quadrilateral. vertices must contain the coordinates of the 4 vertices
// ordered in a counterclosewise fashion
void Renderer::drawQuad(const double vertices[4][2], rgb_hex rgb) const
{
    setColour(rgb);

    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++)
    {
        glVertex2d(vertices[i][0] * pixelScale, vertices[i][1] * pixelScale);
    }
    glEnd();
}

void Renderer::drawRect(double x, double y, double width, double height, rgb_hex rgb) const
{
    double scaledX = x * pixelScale;
    double scaledY = y * pixelScale;
    double scaledWidth = width * pixelScale;
    double scaledHeight = height * pixelScale;

    setColour(rgb);

    glBegin(GL_QUADS);
    glVertex2d(scaledX, scaledY);                              // Top-left vertex
    glVertex2d(scaledX, scaledY + scaledHeight);               // Bottom-left vertex
    glVertex2d(scaledX + scaledWidth, scaledY + scaledHeight); // Bottom-right vertex
    glVertex2d(scaledX + scaledWidth, scaledY);                // Top-right vertex
    glEnd();
}

void Renderer::drawFilledCircle(double x, double y, double r, rgb_hex rgb) const
{
    setColour(rgb);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(x * pixelScale, y * pixelScale);
    for (int i = 0; i <= CIRCLE_SEGMENT_NUM; i++)
    {
        double theta = 2.0 * M_PI * (double)i / (double)CIRCLE_SEGMENT_NUM;
        double vx = r * cos(theta);                               // x-coordinate of current vertex
        double vy = r * sin(theta);                               // y-coordinate of current vertex
        glVertex2d((x + vx) * pixelScale, (y + vy) * pixelScale); // Draw vertex
    }
    glEnd();
}

void Renderer::drawHollowCircle(double x, double y, double r, rgb_hex rgb) const
{
    setColour(rgb);

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < CIRCLE_SEGMENT_NUM; i++)
    {
        double theta = 2.0 * M_PI * (double)i / (double)CIRCLE_SEGMENT_NUM;
        double vx = r * cos(theta); // x-coordinate of current vertex
        double vy = r * sin(theta); // y-coordinate of current vertex

        glVertex2d((x + vx) * pixelScale, (y + vy) * pixelScale); // Draw vertex
    }
    glEnd();
}

void Renderer::setColour(rgb_hex rgb) const
{
    int red = (rgb >> 16) & 0xFF;
    int green = (rgb >> 8) & 0xFF;
    int blue = rgb & 0xFF;
    glColor3ub(red, green, blue);
}