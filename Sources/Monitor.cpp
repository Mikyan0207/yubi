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
}

void Monitor::HandleWindowSwap(SwapDirection direction)
{
    auto hwnd = GetForegroundWindow();
    
    if (hwnd == nullptr)
        return;
    
    auto* node = Display->WindowNode_GetFromWindowHandle(hwnd);
    
    if (node == nullptr)
        return;
    
    if (direction == SwapDirection::CW)
    {
        Display->WindowNode_Swap(node, Display->WindowNode_GetLastLeaf(Display->Root));
    }
    else if (direction == SwapDirection::CCW)
    {
        Display->WindowNode_Swap(node, Display->WindowNode_GetFirstLeaf(Display->Root));
    }
    Display->UpdateScreen();
}

void Monitor::HandleWindowDestroy(HWND hwnd)
{
    if (hwnd == nullptr)
        return;
    
    if (Display->RemoveWindow(hwnd))
        Display->UpdateScreen();
}