#ifndef _WINDOW_H
#define _WINDOW_H

#include <Windows.h>

struct Window
{
    HWND Handle;
    WCHAR* Title;
    RECT Rect;
};

#endif //_WINDOW_H
