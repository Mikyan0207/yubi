#ifndef _WINDOW_H
#define _WINDOW_H

#include <Windows.h>
#include "Types.h"
#include "Monitor.h"

enum class SplitMode
{
    NONE,
    HORIZONTAL,
    VERTICAL
};

enum class WindowSide
{
    NONE,
    LEFT,
    RIGHT
};

struct Window
{
    Window(u32 id)
    {
        Id = id;
    }
    
    u32 Id;
    HWND Handle;
    WCHAR* Title;
    
    bool IsFullScreen;
};

struct WindowNode
{
    Rect<f32> Rect;
    Window* Window; // Temp.
    
    WindowNode* Parent;
    WindowNode* Left;
    WindowNode* Right;
    
    SplitMode Split;
    WindowSide Side = WindowSide::NONE;
};

#endif //_WINDOW_H
