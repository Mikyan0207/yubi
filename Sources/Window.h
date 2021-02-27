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
    Rect<i32> Rect;
    Window* Window; // Temp.
    
    WindowNode* Parent;
    WindowNode* Left;
    WindowNode* Right;
    
    SplitMode Split;
    WindowSide Side;
};

#endif //_WINDOW_H
