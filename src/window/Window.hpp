#pragma once

class GLFWwindow;

class Window {
public:
    static GLFWwindow* window;
    static int Initialize(int width, int height, const char* title);
    static void Terminate();

    static void SetCursorMode(int mode);
    static bool ShouldClose();
    static void ShouldClose(bool close);
    static void SwapBuffers();
    static int width;
    static int height;
};