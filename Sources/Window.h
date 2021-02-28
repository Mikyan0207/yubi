#ifndef _WINDOW_H
#define _WINDOW_H

#include <Windows.h>
#include "yubi.h"
#include "Types.h"
#include "Rect.h"

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

struct YUBI_API Window
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

struct YUBI_API WindowNode
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
