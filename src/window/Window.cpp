#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Window.hpp"

GLFWwindow* Window::window;
int Window::width = 0;
int Window::height = 0;

int Window::Initialize(int width, int height, const char* title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if(!window) {
        std::cerr << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, width, height);

    Window::width = width;
    Window::height = height;
    return 0;
}

void Window::Terminate() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::SetCursorMode(int mode) {
    glfwSetInputMode(window, GLFW_CURSOR, mode);
}

bool Window::ShouldClose()
{
    return glfwWindowShouldClose(window);
}

void Window::ShouldClose(bool close) {
    glfwSetWindowShouldClose(window, close);
}

void Window::SwapBuffers() {
    glfwSwapBuffers(window);
}
