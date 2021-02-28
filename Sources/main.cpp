#include <iostream>
#include <vector>
#include <Windows.h>

#include "Window.h"
#include "Screen.h"
#include "Monitor.h"

// NOTE(Mikyan): Somehow, I can't get the RECT for certain window like
// Edge for example.

// TODO(Mikyan): Remove std::vector<Monitor*>, use custom allocator instead of new?
// Force redraw when resizing windows.
// Handle negative values when moving windows.

static std::vector<Monitor*> Monitors;

static const WCHAR* IgnoredWindows[] = {
    L"Program Manager",
    L"Microsoft Text Input Application"
};

static u32 WindowIdx = 0;
static u32 MonitorIdx = 0;

static Monitor* GetMonitorFromWindow(HWND window, DWORD flags)
{
    auto win32Monitor = MonitorFromWindow(window, flags);
    
    MONITORINFOEX info{};
    info.cbSize = sizeof(MONITORINFOEX);
    
    if (!GetMonitorInfoA(win32Monitor, &info))
    {
        return nullptr;
    }
    
    
    for (const auto& monitor : Monitors)
    {
        if (strcmp(monitor->MonitorName, info.szDevice) == 0)
            return monitor;
    }
    
    return nullptr;
}

BOOL CALLBACK EnumWindowCallback(HWND window, LPARAM)
{
    const auto length = GetWindowTextLengthW(window);
    
    // NOTE(Mikyan): We don't want to stop until we've checked all
    // windows.
    if (!IsWindowVisible(window) || length == 0)
        return TRUE;
    
    Window* w = new Window(WindowIdx++);
    
    w->Handle = window;
    w->Title = new WCHAR[length+1]();
    GetWindowTextW(window, w->Title, length+1);
    
    for(const auto& iTitle : IgnoredWindows)
    {
        if (wcscmp(w->Title, iTitle) == 0)
        {
            delete w;
            return TRUE;
        }
    }
    
    // NOTE(Mikyan): If we fail to get the corresponding Monitor, we take the nearest
    // from this window if we can, otherwise we return nullptr.
    auto* monitor = GetMonitorFromWindow(w->Handle, MONITOR_DEFAULTTONEAREST);
    
    if (monitor != nullptr)
        monitor->Display->AddWindow(w);
    
    return TRUE;
}

BOOL CALLBACK EnumMonitorsCallback(HMONITOR monitor, HDC, LPRECT, LPARAM)
{
    MONITORINFOEX info{};
    info.cbSize = sizeof(MONITORINFOEX);
    
    if (!GetMonitorInfoA(monitor, &info))
    {
        return TRUE;
    }
    
    auto* nMonitor = new Monitor();
    nMonitor->MonitorIndex = MonitorIdx++;
    nMonitor->MonitorName = new CHAR[CCHDEVICENAME+1]();
    memcpy(nMonitor->MonitorName, info.szDevice, CCHDEVICENAME);
    nMonitor->MonitorArea = Rect<i32>(info.rcMonitor.right, info.rcMonitor.bottom);
    nMonitor->WorkingArea = Rect<i32>(info.rcWork.right, info.rcWork.bottom);
    nMonitor->Display = new Screen(nMonitor->WorkingArea.Width, nMonitor->WorkingArea.Height);
    
    Monitors.emplace_back(nMonitor);
    
    return TRUE;
}

int main()
{
    EnumDisplayMonitors(nullptr, nullptr, EnumMonitorsCallback, NULL);
    EnumWindows(EnumWindowCallback, NULL);
    
    for(const auto& monitor : Monitors)
    {
        monitor->Display->UpdateScreen();
    }
    
    // TODO(Mikyan): Infinite Loop.
}