#ifndef _SCREEN_H
#define _SCREEN_H

#include "yubi.h"
#include "Window.h"

enum class DisplayMode
{
    DEFAULT,
    BSP,
    // FLOAT,
    // STACK
};

// NOTE(Mikyan): At some point, we could replace Window pointers by IDs?
struct YUBI_API Screen
{
    Screen(i32 width, i32 height);
    
    void AddWindow(Window* window);
    bool RemoveWindow(HWND windowHandle);
    
    void UpdateScreen();
    void UpdateWindowNode(WindowNode* node);
    void UpdateWindow(HDWP wp, WindowNode* node);
    
    void Tree_CreateNode(WindowNode* parent, Window* window);
    void Tree_Delete(WindowNode* root);
    
    void        WindowNode_Split(WindowNode* node);
    void        WindowNode_Swap(WindowNode* a, WindowNode* b);
    WindowNode* WindowNode_GetFirstLeaf(WindowNode* node);
    WindowNode* WindowNode_GetLastLeaf(WindowNode* node);
    WindowNode* WindowNode_GetNextLeaf(WindowNode* node);
    WindowNode* WindowNode_GetPrevLeaf(WindowNode* node);
    WindowNode* WindowNode_GetFromWindowHandle(HWND handle);
    bool        WindowNode_IsLeaf(WindowNode* node);
    bool        WindowNode_IsOccupied(WindowNode* node);
    bool        WindowNode_IsLeftChild(WindowNode* node);
    bool        WindowNode_IsRightChild(WindowNode* node);
    bool        WindowNode_IsChild(WindowNode* node);
    WindowSide  WindowNode_GetWindowSide(WindowNode* node);
    void        WindowNode_Dump(WindowNode* node);
    
    Rect<i32> ScreenSize;
    u32 WindowCount;
    WindowNode* Root;
    
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
