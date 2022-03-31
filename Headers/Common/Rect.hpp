#pragma once
#include <cstdint>
#include <Windows.h>

namespace yubi
{
    template<typename T>
    struct Rect
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
}
