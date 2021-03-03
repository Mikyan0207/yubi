#include "Monitor.h"

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