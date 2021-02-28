#include <cstdio>
#include "Screen.h"

Screen::Screen(i32 width, i32 height)
{
    Mode = DisplayMode::BSP;
    ScreenSize = Rect<i32>(width, height);
    ScreenRatio = (float)ScreenSize.Width / ScreenSize.Height;
    
    Ratio = 0.5f;
    Gap = 0.0f; 
    
    Root = new WindowNode();
    WindowCount = 0;
    
    Root->Rect = Rect<f32>((f32)width, (f32)height);
}

void Screen::AddWindow(Window* window)
{
    if (!IsNodeOccupied(Root) && IsLeafNode(Root))
    {
        Root->Window = window;
        Root->Parent = nullptr;
        WindowCount += 1;
    }
    else if (Mode == DisplayMode::BSP)
    {
        auto* leaf = GetFirstLeaf(Root);
        CreateWindowNodes(leaf, window);
        WindowCount += 1;
    }
}

void Screen::CreateWindowNodes(WindowNode* node, Window* window)
{
    // NOTE(Mikyan): memset?
    WindowNode* left = (WindowNode*)VirtualAlloc(0, sizeof(WindowNode), MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
    WindowNode* right = (WindowNode*)VirtualAlloc(0, sizeof(WindowNode), MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
    
    if (GetWindowSide(node) == WindowSide::RIGHT)
    {
        right->Window = window;
        left->Window = node->Window;
    }
    else
    {
        left->Window = window;
        right->Window = node->Window;
    }
    
    node->Window = nullptr;
    right->Parent = node;
    left->Parent = node;
    
    node->Right = right;
    node->Left = left;
    
    Split(node);
}

void Screen::Split(WindowNode* node)
{
    if (node->Split == SplitMode::NONE)
    {
        node->Split = node->Rect.Width / node->Rect.Height >= ScreenRatio ? SplitMode::VERTICAL : SplitMode::HORIZONTAL;
    }
    
    
    if (node->Split == SplitMode::VERTICAL)
    {
        node->Left->Rect = node->Rect;
        node->Left->Rect.Width *= Ratio;
        node->Left->Rect.Width -= Gap;
        
        node->Right->Rect = node->Rect;
        node->Right->Rect.X += (node->Rect.Width * Ratio);
        node->Right->Rect.Width *= (1 - Ratio);
        node->Right->Rect.X += Gap;
        node->Right->Rect.Width -= Gap;
    }
    else
    {
        node->Left->Rect = node->Rect;
        node->Left->Rect.Height *= Ratio;
        node->Left->Rect.Height -= Gap;
        
        node->Right->Rect = node->Rect;
        node->Right->Rect.Y += (node->Rect.Height * Ratio);
        node->Right->Rect.Height *= (1 - Ratio);
        node->Right->Rect.Y += Gap;
        node->Right->Rect.Height -= Gap;
    }
}

void Screen::UpdateScreen()
{
    HDWP wp = BeginDeferWindowPos(WindowCount);
    
    UpdateWindow(wp, Root);
    
    EndDeferWindowPos(wp);
}

void Screen::UpdateWindow(HDWP wp, WindowNode* node)
{
    if (wp && node->Window)
    {
        wp = DeferWindowPos(wp, node->Window->Handle, HWND_TOP, (i32)node->Rect.X, (i32)node->Rect.Y,
                            (i32)node->Rect.Width, (i32)node->Rect.Height, SWP_NOCOPYBITS);
    }
    
    if (node->Left)
        UpdateWindow(wp, node->Left);
    if (node->Right)
        UpdateWindow(wp, node->Right);
}

void Screen::Dump(WindowNode* node)
{
    if (node->Left)
        Dump(node->Left);
    if (node->Right)
        Dump(node->Right);
    
    if (node->Window)
        printf("[Title] %ws, [Size] %fx%f\n[Split] %d\n[Side]%d\n",node->Window->Title, node->Rect.Width, node->Rect.Height, node->Split, node->Side);
}

WindowNode* Screen::GetFirstLeaf(WindowNode* node)
{
    auto* tmp = node;
    
    while (!IsLeafNode(tmp))
        tmp = tmp->Left;
    
    return tmp;
}

WindowNode* Screen::GetLastLeaf(WindowNode* node)
{
    auto* tmp = node;
    
    while(!IsLeafNode(tmp))
        tmp = tmp->Right;
    
    return tmp;
}

bool Screen::IsLeafNode(WindowNode* node)
{
    return node->Left == nullptr && node->Right == nullptr;
}

bool Screen::IsNodeOccupied(WindowNode* node)
{
    return node->Window != nullptr;
}

WindowSide Screen::GetWindowSide(WindowNode* node)
{
    return node->Side != WindowSide::NONE ? node->Side : WindowSide::RIGHT;
}