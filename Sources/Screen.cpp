#include <cstdio>
#include "Screen.h"

Screen::Screen(i32 width, i32 height)
{
    Mode = DisplayMode::BSP;
    ScreenSize = Rect<i32>(width, height);
    Gap = 1;
    Root = new WindowNode();
    WindowCount = 0;
    
    Root->Rect = Rect<f32>((f32)width, (f32)height);
}

void Screen::AddWindow(Window* window)
{
    if (IsNodeEmpty(Root))
    {
        Root->Window = window;
        Root->Parent = nullptr;
        WindowCount += 1;
    }
    else if (Mode == DisplayMode::BSP)
    {
        WindowCount += 1;
        auto* leaf = GetFirstLeaf(Root);
        CreateWindowNodes(leaf, window);
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
    }
    else
    {
        left->Window = window;
    }
    
    right->Parent = node;
    left->Parent = node;
    
    node->Right = right;
    node->Left = left;
    
    Split(node);
}

void Screen::Split(WindowNode* node)
{
    f32 ratio = 0.5f;
    bool isRoot = IsRootNode(node);
    
    if (node->Split == SplitMode::NONE)
    {
        node->Split = node->Rect.Width / node->Rect.Height >= 1.16f ? SplitMode::VERTICAL : SplitMode::HORIZONTAL;
    }
    
    
    if (node->Split == SplitMode::VERTICAL)
    {
        node->Left->Rect = node->Rect;
        node->Left->Rect.Width *= ratio;
        node->Left->Rect.Width -= Gap;
        
        node->Right->Rect = node->Rect;
        node->Right->Rect.X += (node->Rect.Width * ratio);
        node->Right->Rect.Width *= (1 - ratio);
        node->Right->Rect.X += Gap;
        node->Right->Rect.Width -= Gap;
    }
    else
    {
        node->Left->Rect = node->Rect;
        node->Left->Rect.Height *= ratio;
        node->Left->Rect.Height -= Gap;
        
        node->Right->Rect = node->Rect;
        node->Right->Rect.Y += (node->Rect.Height * ratio);
        node->Right->Rect.Height *= (1 - ratio);
        node->Right->Rect.Y += Gap;
        node->Right->Rect.Height -= Gap;
    }
    
    // TODO(Mikyan): That's bad. Refactor this part!
    if (isRoot)
    {
        if (node->Left->Window)
        {
            node->Right->Window = node->Window;
        }
        else if (node->Right->Window)
        {
            node->Left->Window = node->Window;
        }
        node->Window = nullptr;
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
                            (i32)node->Rect.Width, (i32)node->Rect.Height, 0);
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
    if (node->Right == nullptr || node->Left == nullptr)
        return node;
    
    if (node->Left != nullptr)
        return GetFirstLeaf(node->Left);
    
    if (node->Right != nullptr)
        return GetFirstLeaf(node->Right);
    
    return nullptr;
}

bool Screen::IsNodeEmpty(WindowNode* node)
{
    return node->Window == nullptr;
}

bool Screen::IsRootNode(WindowNode* node)
{
    return node->Parent == nullptr;
}

WindowSide Screen::GetWindowSide(WindowNode* node)
{
    return node->Side != WindowSide::NONE ? node->Side : WindowSide::RIGHT;
}