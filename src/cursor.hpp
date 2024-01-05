#include <GLFW/glfw3.h>
#include "window.hpp"

class Cursor
{
public:
    void setPixelScale(int scale);
    void getCursorPos(GLFWwindow *window, double *x, double *y);
    void getScaledCursorPos(GLFWwindow *window, double *x, double *y);

private:
    int pixelScale;
};