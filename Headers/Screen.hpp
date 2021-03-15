#ifndef _SCREEN_H
#define _SCREEN_H

#include "yubi.h"
#include "Window.h"
#include <algorithm>

enum class DisplayMode
{
    DEFAULT,
    BSP,
    FLOAT,
    STACK
};

struct YUBI_API Screen
{
    Screen(i32 width, i32 height);
    
    void AddWindow(Window* window);
    bool RemoveWindow(HWND windowHandle);
    
    void UpdateScreen();
    void UpdateWindowNode(Window* node);
    void UpdateWindow(HDWP wp, Window* node);
    
    void Tree_CreateNode(Window* parent, Window* window);
    void Tree_Delete(Window* root);
    
    void        Window_Split(Window* node);
    void        Window_Swap(Window* a, Window* b);
    Window*     Window_GetFirstLeaf(Window* node);
    Window*     Window_GetLastLeaf(Window* node);
    Window*     Window_GetNextLeaf(Window* node);
    Window*     Window_GetPrevLeaf(Window* node);
    Window*     Window_GetFromWindowHandle(HWND handle);
    bool        Window_IsLeaf(Window* node);
    bool        Window_IsOccupied(Window* node);
    bool        Window_IsLeftChild(Window* node);
    bool        Window_IsRightChild(Window* node);
    bool        Window_IsChild(Window* node);
    WindowSide  Window_GetWindowSide(Window* node);
    void        Window_Dump(Window* node);
    bool        Window_IsFullScreen(Window* node);
    
    Rect<f32> ScreenSize;
    u32 WindowCount;
    Window* Root;
    
    // NOTE(Mikyan): 16:9, 21:9, ...
    f32 ScreenRatio;
    f32 Ratio;
    f32 Gap;
    u32 PaddingTop;
    u32 PaddingLeft;
    u32 PaddingRight;
    u32 PaddingBottom;
    
    DisplayMode Mode;
};


#endif //_SCREEN_H
