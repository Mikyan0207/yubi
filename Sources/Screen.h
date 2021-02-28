#ifndef _SCREEN_H
#define _SCREEN_H

#include "Window.h"

enum class DisplayMode
{
    DEFAULT,
    BSP,
    // FLOAT,
    // STACK
};

// NOTE(Mikyan): At some point, we'll could replace Window pointers by IDs?
struct Screen
{
    Screen(i32 width, i32 height);
    
    void AddWindow(Window* window);
    void CreateWindowNodes(WindowNode* node, Window* window);
    void Split(WindowNode*);
    void UpdateScreen();
    void UpdateWindow(HDWP wp, WindowNode* node);
    void Dump(WindowNode* node);
    bool IsNodeEmpty(WindowNode* node);
    bool IsRootNode(WindowNode* node);
    WindowSide GetWindowSide(WindowNode* node);
    WindowNode* GetFirstLeaf(WindowNode* node);
    
    Rect<i32> ScreenSize;
    u32 WindowCount;
    WindowNode* Root;
    
    // NOTE(Mikyan): We may need a ratio when resizing windows.
    
    u32 Gap;
    u32 PaddingTop;
    u32 PaddingLeft;
    u32 PaddingRight;
    u32 PaddingBottom;
    
    DisplayMode Mode;
};

template<typename T>
bool IsInRange(T value, T min, T max)
{
    if (value >= min && value <= max)
        return true;
    return false;
}


#endif //_SCREEN_H
