#ifndef _VECTOR_H
#define _VECTOR_H

#include "yubi.h"

template<typename T>
struct YUBI_API Vector2
{
    Vector2() = default;
    
    Vector2(T x, T y)
    {
        X = x;
        Y = y;
    }
    
    T X;
    T Y;
};
#endif //_VECTOR_H
