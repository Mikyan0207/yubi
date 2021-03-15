#ifndef _WINDOW_H
#define _WINDOW_H

#include <Windows.h>
#include <string>
#include "yubi.h"
#include "Rect.h"
#include "Types.h"

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
    HWND Handle;
    std::wstring Title;
    
    Rect<f32> Area;
    Rect<i32> FrameBounds;
    Rect<f32> Offset;
    
    Window* Parent;
    Window* Left;
    Window* Right;
    
    bool IsFullScreen;
    bool IsMaximized;
    bool IsMinimized;
    
    SplitMode Split;
    WindowSide Side = WindowSide::NONE;
};

#endif //_WINDOW_H
