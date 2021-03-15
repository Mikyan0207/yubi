#ifndef _RECT_H
#define _RECT_H

#include <Windows.h>
#include "yubi.h"

template<typename T>
struct YUBI_API Rect
{
    Rect() = default;
    
    Rect(T x, T y, T width, T height)
    {
        X = x;
        Y = y;
        Width = width;
        Height = height;
    }
    
    Rect(T width, T height)
    {
        Width = width;
        Height = height;
    }
    
    Rect(const RECT& rect)
    {
        X = static_cast<T>(rect.left);
        Y = static_cast<T>(rect.top);
        Width = static_cast<T>(rect.right - rect.left);
        Height = static_cast<T>(rect.bottom - rect.top);
    }
    
    T X = 0;
    T Y = 0;
    T Width = 0;
    T Height = 0;
};

#endif //_RECT_H
