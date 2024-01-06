#include <cmath>
#include "renderer.hpp"

void Renderer::init(double left, double right, double bottom, double top, int scale)
{
    pixelScale = scale;
    pixelScaleHalf = pixelScale / 2;

    glPointSize((float)pixelScale);
    glOrtho(left, right, bottom, top, -1.0, 1.0);
    glClearColor(0.2, 0.2, 0.2, 0);
}

void Renderer::setColour(int red, int green, int blue)
{
    glColor3ub(red, green, blue);
}

void Renderer::setColour(float red, float green, float blue)
{
    glColor3f(red, green, blue);
}

void Renderer::clearScreen()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::drawPixel(double x, double y)
{
    glBegin(GL_POINTS);
    glVertex2d(x * pixelScale, y * pixelScale);
    glEnd();
}

// Draw a quadrilateral. vertices must contain the coordinates of the 4 vertices
// ordered in a counterclosewise fashion
void Renderer::drawQuad(double vertices[4][2])
{
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++)
    {
        glVertex2d(vertices[i][0] * pixelScale, vertices[i][1] * pixelScale);
    }
    glEnd();
}

void Renderer::drawRect(double x, double y, double width, double height)
{
    double scaledX = x * pixelScale;
    double scaledY = y * pixelScale;
    double scaledWidth = width * pixelScale;
    double scaledHeight = height * pixelScale;

    glBegin(GL_QUADS);
    glVertex2d(scaledX, scaledY); // Top-left vertex
    glVertex2d(scaledX, scaledY + scaledHeight); // Bottom-left vertex
    glVertex2d(scaledX + scaledWidth, scaledY + scaledHeight); // Bottom-right vertex
    glVertex2d(scaledX + scaledWidth, scaledY); // Top-right vertex
    glEnd();
}

void Renderer::drawFilledCircle(double x, double y, double r, int numSegments)
{
    if (!numSegments)
    {
        return;
    }

    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(x * pixelScale, y * pixelScale);
    for (int i = 0; i <= numSegments; i++)
    {
        double theta = 2.0 * M_PI * (double)i / (double)numSegments;
        double vx = r * cos(theta); // x-coordinate of current vertex
        double vy = r * sin(theta); // y-coordinate of current vertex
        glVertex2d((x + vx) * pixelScale, (y + vy) * pixelScale); // Draw vertex
    }
    glEnd();
}

void Renderer::drawHollowCircle(double x, double y, double r, int numSegments)
{
    if (!numSegments)
    {
        return;
    }

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < numSegments; i++)
    {
        double theta = 2.0 * M_PI * (double)i / (double)numSegments;
        double vx = r * cos(theta); // x-coordinate of current vertex
        double vy = r * sin(theta); // y-coordinate of current vertex

        glVertex2d((x + vx) * pixelScale, (y + vy) * pixelScale); // Draw vertex
    }
    glEnd();

}