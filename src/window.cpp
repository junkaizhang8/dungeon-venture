#include "window.hpp"

void Window::run()
{
    update();
    display();
}

void Window::close()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}