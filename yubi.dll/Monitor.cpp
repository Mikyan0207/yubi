#include "Monitor.h"

void Monitor::HandleClockwiseRotation()
{
    auto hwnd = GetForegroundWindow();
    
    if (hwnd == nullptr)
        return;
    
    auto* node = Display->WindowNode_GetFromWindowHandle(hwnd);
    
    if (node == nullptr)
        return;
    
    auto* prevNode = Display->WindowNode_GetPrevLeaf(node);
    
    if (prevNode == nullptr)
        return;
    
    Display->WindowNode_Swap(node, prevNode);
    Display->UpdateScreen();
}

void Monitor::HandleCounterClockwiseRotation()
{
    auto hwnd = GetForegroundWindow();
    
    if (hwnd == nullptr)
        return;
    
    auto* node = Display->WindowNode_GetFromWindowHandle(hwnd);
    
    if (node == nullptr)
        return;
    
    auto* nextNode = Display->WindowNode_GetNextLeaf(node);
    
    if (nextNode == nullptr)
        return;
    
    Display->WindowNode_Swap(node, nextNode);
    Display->UpdateScreen();
}

void Monitor::HandleWindowDestroy(HWND hwnd)
{
    if (hwnd == nullptr)
        return;
    
    if (Display->RemoveWindow(hwnd))
        Display->UpdateScreen();
}

void Monitor::HandleSplitDirection()
{
    auto hwnd = GetForegroundWindow();
    
    if (hwnd == nullptr)
        return;
    
    auto* node = Display->WindowNode_GetFromWindowHandle(hwnd);
    
    if (node == nullptr)
        return;
    
    if (node->Split == SplitMode::NONE)
        return;
    
    if (node->Split == SplitMode::VERTICAL)
    {
        // TODO(Mikyan): Force horizontal split
    }
    else
    {
        // TODO(Mikyan): Force vertical split
    }
}
