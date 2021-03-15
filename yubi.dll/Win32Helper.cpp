#include "Win32Helper.h"

LONG Win32Helper::GetWindowStyles(HWND window)
{
    return GetWindowLongW(window, GWL_STYLE);
}

LONG Win32Helper::GetWindowExStyles(HWND window)
{
    return GetWindowLongW(window, GWL_EXSTYLE);
}

bool Win32Helper::WindowIsValid(HWND window)
{
    auto exStyles = Win32Helper::GetWindowExStyles(window);
    auto styles = Win32Helper::GetWindowStyles(window);
    
    return IsWindowVisible(window) && !((exStyles & WS_EX_NOACTIVATE) == WS_EX_NOACTIVATE) && !((styles & WS_CHILD) == WS_CHILD);
}

bool Win32Helper::WinEventWindowIsValid(LONG objectId, LONG childId, HWND hwnd)
{
    return objectId == OBJID_WINDOW && childId == CHILDID_SELF && hwnd != nullptr;
}

bool Win32Helper::WindowIsCloaked(HWND window)
{
    bool isCloaked = false;
    
    DwmGetWindowAttribute(window, DWMWA_CLOAKED, &isCloaked, sizeof(bool));
    
    return isCloaked;
}

Rect<i32> Win32Helper::WindowGetFrameBounds(HWND window)
{
    RECT rect = {};
    
    DwmGetWindowAttribute(window, DWMWA_EXTENDED_FRAME_BOUNDS, &rect, sizeof(RECT));
    
    return std::move(Rect<i32>(rect));
}