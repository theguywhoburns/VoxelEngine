#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string.h>
#include "Events.hpp"

bool* Events::_keys;
unsigned int* Events::_frames;
unsigned int  Events::_current = 0;
float Events::deltaX = 0.0f;
float Events::deltaY = 0.0f;
float Events::X = 0.0f;
float Events::Y = 0.0f;
bool  Events::_cursor_locked = false;
bool  Events::_cursor_started= false;

#define _MOUSE_BUTTONS_INDEX 1024

void window_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    Window::width = width;
    Window::height = height;
}

void cursor_position_callback(GLFWwindow* window, double xPos, double yPos) {
    if(Events::_cursor_locked) {
        Events::deltaX += xPos-Events::X;
        Events::deltaY += yPos-Events::Y;
    } else {
        Events::_cursor_started = true;
    }
    Events::X = xPos;
    Events::Y = yPos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mode) {
    if(action == GLFW_PRESS) {
        Events::_keys[_MOUSE_BUTTONS_INDEX + button] = true;
        Events::_frames[_MOUSE_BUTTONS_INDEX + button] = Events::_current;
    } else if(action == GLFW_RELEASE){
        Events::_keys[_MOUSE_BUTTONS_INDEX + button] = false;
        Events::_frames[_MOUSE_BUTTONS_INDEX + button] = Events::_current;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if(action == GLFW_PRESS) {
        Events::_keys[key] = true;
        Events::_frames[key] = Events::_current;
    } else if(action == GLFW_RELEASE){
        Events::_keys[key] = false;
        Events::_frames[key] = Events::_current;
    }
}

int Events::Initialize() {
    GLFWwindow* window = Window::window;
    _keys = new bool[1032];
    _frames = new unsigned int[1032];

    memset(_keys, false, 1032);
    memset(_frames, 0, 1032);

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    return 0;
}

void Events::PullEvents() {
    _current++;
    deltaX = 0.0f;
    deltaY = 0.0f;
    glfwPollEvents();
}

void Events::Terminate() {
    delete _keys;
    delete _frames;
}

bool Events::Pressed(int keycode) {
    if(keycode < 0 || keycode >= _MOUSE_BUTTONS_INDEX) return false;
    return _keys[keycode];
}

bool Events::JPressed(int keycode) {
    if(keycode < 0 || keycode >= _MOUSE_BUTTONS_INDEX) return false;
    return _keys[keycode] && _frames[keycode] == _current;
}

bool Events::Clicked(int button) {
    return _keys[_MOUSE_BUTTONS_INDEX + button];
}

bool Events::JClicked(int button) {
    return _keys[_MOUSE_BUTTONS_INDEX + button] 
    && _frames[_MOUSE_BUTTONS_INDEX + button] == _current;
}

void Events::ToggleCursor() {
    _cursor_locked = !_cursor_locked;
    Window::SetCursorMode(_cursor_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}