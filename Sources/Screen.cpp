#include <cstdio>
#include "Screen.h"

Screen::Screen(i32 width, i32 height)
{
    Mode = DisplayMode::BSP;
    ScreenSize = Rect<i32>(width, height);
    ScreenRatio = (float)ScreenSize.Width / ScreenSize.Height;
    
    Ratio = 0.5f;
    Gap = 0.0f; 
    
    Root = (WindowNode*)VirtualAlloc(0, sizeof(WindowNode), MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
    WindowCount = 0;
    
    Root->Rect = Rect<f32>((f32)width, (f32)height);
}

void Screen::AddWindow(Window* window)
{
    if (!WindowNode_IsOccupied(Root) && WindowNode_IsLeaf(Root))
    {
        Root->Window = window;
        Root->Parent = nullptr;
        WindowCount += 1;
    }
    else if (Mode == DisplayMode::BSP)
    {
        auto* leaf = WindowNode_GetFirstLeaf(Root);
        Tree_CreateNode(leaf, window);
        WindowCount += 1;
    }
}

void Screen::RemoveWindow(HWND handle)
{
    // TODO(Mikyan): Remove node from tree and reorganize tree structure.
    auto* node = WindowNode_GetFromWindowHandle(Root, handle);
    
    if (node == nullptr)
    {
        // TODO(Mikyan): Logging.
        return;
    }
    
    // @Clean-up this?
    if (WindowNode_IsLeftChild(node))
    {
        // NOTE(Mikyan): ??
        memcpy(node->Parent, node->Parent->Right, sizeof(WindowNode));
        
        if (!VirtualFree(node, 0, MEM_RELEASE))
        {
            // TODO(Mikyan): Logging.
            return;
        }
        
        if (!VirtualFree(node->Parent->Right, 0, MEM_RELEASE))
        {
            // TODO(Mikyan): Logging.
            return;
        }
    }
    else if (WindowNode_IsRightChild(node))
    {
        memcpy(node->Parent, node->Parent->Left, sizeof(WindowNode));
        
        if (!VirtualFree(node, 0, MEM_RELEASE))
        {
            // TODO(Mikyan): Logging.
            return;
        }
        
        if (!VirtualFree(node->Parent->Left, 0, MEM_RELEASE))
        {
            // TODO(Mikyan): Logging.
            return;
        }
    }
    else
    {
        // NOTE(Mikyan): Root node?
        if (!VirtualFree(node, 0, MEM_RELEASE))
        {
            // TODO(Mikyan): Logging.
            return;
        }
    }
    
    WindowCount -= 1;
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

void Screen::Tree_CreateNode(WindowNode* parent, Window* window)
{
    WindowNode* left = (WindowNode*)VirtualAlloc(0, sizeof(WindowNode), MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
    WindowNode* right = (WindowNode*)VirtualAlloc(0, sizeof(WindowNode), MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
    
    if (WindowNode_GetWindowSide(parent) == WindowSide::RIGHT)
    {
        right->Window = window;
        left->Window = parent->Window;
    }
    else
    {
        left->Window = window;
        right->Window = parent->Window;
    }
    
    parent->Window = nullptr;
    right->Parent = parent;
    right->Side = WindowSide::RIGHT;
    left->Parent = parent;
    left->Side = WindowSide::LEFT;
    
    parent->Right = right;
    parent->Left = left;
    
    WindowNode_Split(parent);
}

void Screen::Tree_Delete(WindowNode* root)
{
    if (root)
    {
        if (root->Left)
            Tree_Delete(root->Left);
        if (root->Right)
            Tree_Delete(root->Right);
        
        if (!VirtualFree(root, 0, MEM_RELEASE))
        {
            // NOTE(Mikyan): Logging.
        }
    }
}

void Screen::WindowNode_Split(WindowNode* node)
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

void Screen::WindowNode_Swap(WindowNode* a, WindowNode* b)
{
    // NOTE(Mikyan): We only swap the window handles?
    auto* tmp = a->Window;
    a->Window = b->Window;
    b->Window = tmp;
}

WindowNode* Screen::WindowNode_GetFirstLeaf(WindowNode* node)
{
    auto* tmp = node;
    
    while (!WindowNode_IsLeaf(tmp))
        tmp = tmp->Left;
    
    return tmp;
}

WindowNode* Screen::WindowNode_GetLastLeaf(WindowNode* node)
{
    auto* tmp = node;
    
    while(!WindowNode_IsLeaf(tmp))
        tmp = tmp->Right;
    
    return tmp;
}

WindowNode* Screen::WindowNode_GetFromWindowHandle(WindowNode* node, HWND handle)
{
    if (node->Window && node->Window->Handle == handle)
        return node;
    
    if (node->Left)
        return WindowNode_GetFromWindowHandle(node->Left, handle);
    
    if (node->Right)
        return WindowNode_GetFromWindowHandle(node->Right, handle);
    
    return nullptr;
}

bool Screen::WindowNode_IsLeaf(WindowNode* node)
{
    return node->Left == nullptr && node->Right == nullptr;
}

bool Screen::WindowNode_IsOccupied(WindowNode* node)
{
    return node->Window != nullptr;
}

bool Screen::WindowNode_IsLeftChild(WindowNode* node)
{
    if (node->Side != WindowSide::NONE)
        return node->Side == WindowSide::LEFT;
    
    if (!node->Parent)
        return false;
    
    return node->Parent->Left == node;
}

bool Screen::WindowNode_IsRightChild(WindowNode* node)
{
    if (node->Side != WindowSide::NONE)
        return node->Side == WindowSide::RIGHT;
    
    if (!node->Parent)
        return false;
    
    return node->Parent->Right == node;
}

WindowSide Screen::WindowNode_GetWindowSide(WindowNode* node)
{
    return node->Side != WindowSide::NONE ? node->Side : WindowSide::RIGHT;
}


void Screen::WindowNode_Dump(WindowNode* node)
{
    // TODO(Mikyan): Dump node information.
}
