#include "Screen.hpp"

Screen::Screen(i32 width, i32 height)
{
    Mode = DisplayMode::BSP;
    ScreenSize = Rect<f32>(static_cast<f32>(width), static_cast<f32>(height));
    ScreenRatio = (float)ScreenSize.Width / ScreenSize.Height;
    
    Ratio = 0.5f;
    Gap = 0.0f;
    
    Root = (Window*)VirtualAlloc(0, sizeof(Window), MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
    WindowCount = 0;
    
    Root->Area = Rect<f32>(static_cast<f32>(width), static_cast<f32>(height));
}

void Screen::AddWindow(Window* window)
{
    printf("New Window: %ws\n", window->Title.c_str());
    
    if (!Window_IsOccupied(Root) && Window_IsLeaf(Root))
    {
        Root = window;
        Root->Parent = nullptr;
        Root->Area = ScreenSize;
        WindowCount += 1;
    }
    else if (Mode == DisplayMode::BSP)
    {
        auto* leaf = Window_GetFirstLeaf(Root);
        Tree_CreateNode(leaf, window);
        WindowCount += 1;
    }
    
    UpdateScreen();
}

bool Screen::RemoveWindow(HWND handle)
{
    // TODO(Mikyan): Remove node from tree and reorganize tree structure.
    auto* node = Window_GetFromWindowHandle(handle);
    
    if (node == nullptr)
    {
        // TODO(Mikyan): Logging.
        return false;
    }
    
    // @Clean-up this?
    if (Window_IsLeftChild(node))
    {
        // NOTE(Mikyan): ??
        memcpy(node->Parent, node->Parent->Right, sizeof(Window));
        
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
    else if (Window_IsRightChild(node))
    {
        memcpy(node->Parent, node->Parent->Left, sizeof(Window));
        
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

void Screen::UpdateWindow(HDWP wp, Window* node)
{
    if (wp && node->Handle)
    {
        wp = DeferWindowPos(wp, node->Handle, NULL,
                            static_cast<i32>(node->Area.X), static_cast<i32>(node->Area.Y),
                            static_cast<i32>(node->Area.Width), static_cast<i32>(node->Area.Height),
                            SWP_NOCOPYBITS | SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER);
        
        //std::cout << "[Window] " << node->Title << " " << "Width: " << node->Area.Width << " Height: " << node->Area.Height << std::endl;
    }
    
    if (node->Left)
        UpdateWindow(wp, node->Left);
    if (node->Right)
        UpdateWindow(wp, node->Right);
}

void Screen::Tree_CreateNode(Window* parent, Window* window)
{
    Window* left = (Window*)VirtualAlloc(0, sizeof(Window), MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
    Window* right = (Window*)VirtualAlloc(0, sizeof(Window), MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
    
    if (Window_GetWindowSide(parent) == WindowSide::RIGHT)
    {
        right->Handle = window->Handle;
        left->Handle = parent->Handle;
    }
    else
    {
        left->Handle = window->Handle;
        right->Handle = parent->Handle;
    }
    
    parent->Handle = nullptr;
    right->Parent = parent;
    right->Side = WindowSide::RIGHT;
    left->Parent = parent;
    left->Side = WindowSide::LEFT;
    
    parent->Right = right;
    parent->Left = left;
    
    Window_Split(parent);
}

void Screen::Tree_Delete(Window* root)
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

void Screen::Window_Split(Window* node)
{
    if (node->Split == SplitMode::NONE)
    {
        node->Split = node->Area.Width / node->Area.Height >= ScreenRatio ? SplitMode::VERTICAL : SplitMode::HORIZONTAL;
    }
    
    if (node->Split == SplitMode::VERTICAL)
    {
        node->Left->Area = node->Area;
        node->Left->Area.Width *= Ratio;
        
        node->Right->Area = node->Area;
        node->Right->Area.X += (node->Area.Width * Ratio);
        node->Right->Area.Width *= (1 - Ratio);
    }
    else
    {
        node->Left->Area = node->Area;
        node->Left->Area.Height *= Ratio;
        
        node->Right->Area = node->Area;
        node->Right->Area.Y += (node->Area.Height * Ratio);
        node->Right->Area.Height *= (1 - Ratio);
    }
    
    
}

void Screen::Window_Swap(Window* a, Window* b)
{
    // NOTE(Mikyan): We only swap the window handles?
    std::swap(a->Handle, b->Handle);
}

Window* Screen::Window_GetFirstLeaf(Window* node)
{
    auto* tmp = node;
    
    while (!Window_IsLeaf(tmp))
        tmp = tmp->Left;
    
    return tmp;
}

Window* Screen::Window_GetLastLeaf(Window* node)
{
    auto* tmp = node;
    
    while(!Window_IsLeaf(tmp))
        tmp = tmp->Right;
    
    return tmp;
}

Window* Screen::Window_GetNextLeaf(Window* node)
{
    if (node->Parent == nullptr)
        return nullptr;
    
    if (Window_IsRightChild(node))
        return Window_GetNextLeaf(node->Parent);
    
    if (Window_IsLeaf(node->Parent->Right))
        return node->Parent->Right;
    
    return Window_GetFirstLeaf(node->Parent->Right->Left);
}

Window* Screen::Window_GetPrevLeaf(Window* node)
{
    if (node->Parent == nullptr)
        return nullptr;
    
    if (Window_IsLeftChild(node))
        return Window_GetPrevLeaf(node);
    
    if (Window_IsLeaf(node->Parent->Left))
        return node->Parent->Left;
    
    return Window_GetFirstLeaf(node->Parent->Left->Right);
}

Window* Screen::Window_GetFromWindowHandle(HWND handle)
{
    auto* n = Window_GetFirstLeaf(Root);
    
    while (n)
    {
        if (n->Handle == handle)
            return n;
        n = Window_GetNextLeaf(n);
    }
    
    return nullptr;
}

bool Screen::Window_IsLeaf(Window* node)
{
    return node->Left == nullptr && node->Right == nullptr;
}

bool Screen::Window_IsOccupied(Window* node)
{
    return node->Handle != nullptr;
}

bool Screen::Window_IsLeftChild(Window* node)
{
    if (node->Side != WindowSide::NONE)
        return node->Side == WindowSide::LEFT;
    
    if (!node->Parent)
        return false;
    
    return node->Parent->Left == node;
}

bool Screen::Window_IsRightChild(Window* node)
{
    if (node->Side != WindowSide::NONE)
        return node->Side == WindowSide::RIGHT;
    
    if (!node->Parent)
        return false;
    
    return node->Parent->Right == node;
}

bool Screen::Window_IsChild(Window* node)
{
    return node != nullptr && node->Parent != nullptr;
}

WindowSide Screen::Window_GetWindowSide(Window* node)
{
    return node->Side != WindowSide::NONE ? node->Side : WindowSide::RIGHT;
}


void Screen::Window_Dump(Window* node)
{
    // TODO(Mikyan): Dump node information.
}

bool Screen::Window_IsFullScreen(Window* node)
{
    if (node == nullptr || node->Handle == nullptr)
        return false;
    
    return node->IsFullScreen;
}
