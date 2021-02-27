#include <cstdio>
#include "Screen.h"

Screen::Screen(i32 width, i32 height)
{
    ScreenSize = Rect<i32>(width, height);
    Gap = 1;
    Root = new WindowNode();
    
    Root->Rect = Rect<i32>(width, height);
}

void Screen::AddWindow(Window* window)
{
    auto* leaf = GetFirstLeaf(Root);
    
    if (leaf->Right == nullptr)
    {
        leaf->Right = new WindowNode();
        leaf->Right->Parent = leaf;
        leaf->Right->Window = window;
        Dump(leaf->Right);
    }
    else if (leaf->Left == nullptr)
    {
        leaf->Left = new WindowNode();
        leaf->Left->Parent = leaf;
        leaf->Left->Window = window;
        Dump(leaf->Left);
    }
    
    Split(leaf);
}

void Screen::Split(WindowNode* node)
{
    float ratio = 0.5f;
    
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
}

void Screen::Dump(WindowNode* node)
{
    printf("[Size] %dx%d\n[Split] %d\n[Side]%d\n", node->Rect.Width, node->Rect.Height, node->Split, node->Side);
}

WindowNode* Screen::GetFirstLeaf(WindowNode* node)
{
    if (node->Right == nullptr || node->Left == nullptr)
        return node;
    
    if (node->Left != nullptr)
        GetFirstLeaf(node->Left);
    
    if (node->Right != nullptr)
        GetFirstLeaf(node->Right);
    
    return nullptr;
}