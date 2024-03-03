#pragma once

#include "Window.hpp"

class Events {
public:
    static bool* _keys;
    static unsigned int* _frames; 
    static unsigned int  _current; 
    static float deltaX;
    static float deltaY;
    static float X;
    static float Y;
    static bool _cursor_locked;
    static bool _cursor_started;

    static int Initialize();
    static void PullEvents();
    static void Terminate();

    static bool Pressed(int keycode);
    static bool JPressed(int keycode);
    static bool Clicked(int button);
    static bool JClicked(int button);

    static void ToggleCursor();
};