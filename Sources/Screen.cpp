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
    //printf("New Window: %ws\n", window->Title);
    
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
    
    UpdateScreen();
}

bool Screen::RemoveWindow(HWND handle)
{
    // TODO(Mikyan): Remove node from tree and reorganize tree structure.
    auto* node = WindowNode_GetFromWindowHandle(handle);
    
    if (node == nullptr)
    {
        // TODO(Mikyan): Logging.
        return false;
    }
    
    // @Clean-up this?
    if (WindowNode_IsLeftChild(node))
    {
        // NOTE(Mikyan): ??
        memcpy(node->Parent, node->Parent->Right, sizeof(WindowNode));
        
        if (!VirtualFree(node->Parent->Right, 0, MEM_RELEASE))
        {
            // TODO(Mikyan): Logging.
            return false;
        }
        
        if (!VirtualFree(node, 0, MEM_RELEASE))
        {
            // TODO(Mikyan): Logging.
            return false;
        }
    }
    else if (WindowNode_IsRightChild(node))
    {
        memcpy(node->Parent, node->Parent->Left, sizeof(WindowNode));
        
        if (!VirtualFree(node->Parent->Left, 0, MEM_RELEASE))
        {
            // TODO(Mikyan): Logging.
            return false;
        }
        
        if (!VirtualFree(node, 0, MEM_RELEASE))
        {
            // TODO(Mikyan): Logging.
            return false;
        }
    }
    else
    {
        // NOTE(Mikyan): Root node?
        if (!VirtualFree(node, 0, MEM_RELEASE))
        {
            // TODO(Mikyan): Logging.
            return false;
        }
    }
    
    WindowCount -= 1;
    
    return true;
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
        wp = DeferWindowPos(wp, node->Window->Handle, NULL, (i32)node->Rect.X, (i32)node->Rect.Y,
                            (i32)node->Rect.Width, (i32)node->Rect.Height, SWP_NOCOPYBITS | SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER);
        
        
        printf("[Window] %ws\n Size: %f - %f\n", node->Window->Title, node->Rect.Width, node->Rect.Height);
        
        // TODO(Mikyan): Window redrawing and update.
        
        //SetFocus(node->Window->Handle);
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
        
        node->Right->Rect = node->Rect;
        node->Right->Rect.X += (node->Rect.Width * Ratio);
        node->Right->Rect.Width *= (1 - Ratio);
    }
    else
    {
        node->Left->Rect = node->Rect;
        node->Left->Rect.Height *= Ratio;
        
        node->Right->Rect = node->Rect;
        node->Right->Rect.Y += (node->Rect.Height * Ratio);
        node->Right->Rect.Height *= (1 - Ratio);
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

WindowNode* Screen::WindowNode_GetNextLeaf(WindowNode* node)
{
    if (node->Parent == nullptr)
        return nullptr;
    
    if (WindowNode_IsRightChild(node))
        return WindowNode_GetNextLeaf(node->Parent);
    
    if (WindowNode_IsLeaf(node->Parent->Right))
        return node->Parent->Right;
    
    return WindowNode_GetFirstLeaf(node->Parent->Right->Left);
}

WindowNode* Screen::WindowNode_GetPrevLeaf(WindowNode* node)
{
    return nullptr;
}

WindowNode* Screen::WindowNode_GetFromWindowHandle(HWND handle)
{
    auto* n = WindowNode_GetFirstLeaf(Root);
    
    while (n)
    {
        if (n->Window && n->Window->Handle == handle)
            return n;
        n = WindowNode_GetNextLeaf(n);
    }
    
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

bool Screen::WindowNode_IsChild(WindowNode* node)
{
    return node != nullptr && node->Parent != nullptr;
}

WindowSide Screen::WindowNode_GetWindowSide(WindowNode* node)
{
    return node->Side != WindowSide::NONE ? node->Side : WindowSide::RIGHT;
}


void Screen::WindowNode_Dump(WindowNode* node)
{
    // TODO(Mikyan): Dump node information.
}

bool Screen::WindowNode_IsFullScreen(WindowNode* node)
{
    if (node == nullptr || node->Window == nullptr)
        return false;
    
    return node->Window->IsFullScreen;
}
