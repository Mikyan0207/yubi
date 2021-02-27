#include <iostream>
#include <vector>
#include <Windows.h>

#include "Window.h"
#include "Screen.h"
#include "Monitor.h"


// NOTE(Mikyan): Somehow, I can't get the RECT for certain window like
// Edge for example.

// TODO(Mikyan): Remove std::vector, use custom allocator instead of new.
static Screen* gScreen;

static u32 WindowIdx = 0;

BOOL CALLBACK EnumWindowCallback(HWND window, LPARAM)
{
    if (window == nullptr)
        return FALSE;
    
    const auto length = GetWindowTextLengthW(window);
    
    // NOTE(Mikyan): We don't want to stop until we've checked all
    // the window.
    if (!IsWindowVisible(window) || length == 0)
        return TRUE;
    
    Window w(WindowIdx++);
    
    w.Handle = window;
    
    /*w.Rect = Rect<i32>(rect.left, 
                       rect.top,
                       rect.right - rect.left,
                       rect.bottom - rect.top);
    */
    
    gScreen->AddWindow(&w);
    
    return TRUE;
}

// NOTE(Mikyan): Just used for testing.
void MoveMultipleWindow()
{
    
    HDWP p = BeginDeferWindowPos(2);
    
    // Move window using DeferWindowPos()
    //p = DeferWindowPos(p, g_Windows[0].Handle, HWND_TOP, 1, 0, center - 1, height - 1, 0);
    //p = DeferWindowPos(p, g_Windows[1].Handle, HWND_TOP, center + 1, 0, center - 1, height - 1, 0);
    
    EndDeferWindowPos(p);
    
}

int main()
{
    HMONITOR monitor = MonitorFromPoint(POINT {0, 0}, MONITOR_DEFAULTTOPRIMARY);
    MONITORINFO info{};
    
    info.cbSize = sizeof(MONITORINFO);
    
    if (!GetMonitorInfoA(monitor, &info))
    {
        std::cout << "Failed to get monitor info." << std::endl;
    }
    
    gScreen = new Screen(info.rcWork.right, info.rcWork.bottom);
    EnumWindows(EnumWindowCallback, NULL);
    
    //MoveMultipleWindow();
}