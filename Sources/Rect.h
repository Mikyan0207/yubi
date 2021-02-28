#ifndef _RECT_H
#define _RECT_H

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
        
        XCenter = Width / (T)2;
        YCenter = Height / (T)2;
    }
    
    Rect(T width, T height)
    {
        Width = width;
        Height = height;
        
        XCenter = Width / (T)2;
        YCenter = Height / (T)2;
    }
    
    T X = 0;
    T Y = 0;
    T Width = 0;
    T Height = 0;
    
    T XCenter = 0;
    T YCenter = 0;
};

#endif //_RECT_H
