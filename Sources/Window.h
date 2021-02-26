#ifndef _WINDOW_H
#define _WINDOW_H

#include <Windows.h>
#include "Types.h"

enum class SplitMode
{
    NONE,
    HORIZONTAL,
    VERTICAL
};

struct Window
{
    HWND Handle;
    WCHAR* Title;
    
    i32 X;
    i32 Y;
    i32 Width;
    i32 Height;
    
    SplitMode Split;
    
    Window* Parent;
    Window* Left;
    Window* Right;
};

#endif //_WINDOW_H
