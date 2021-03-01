#include <vector>
#include <iostream>
#include <Windows.h>

#include "Window.h"
#include "Screen.h"
#include "Monitor.h"

// NOTE(Mikyan): Somehow, I can't get the RECT for certain window like
// Edge for example.

// TODO(Mikyan): Remove std::vector<Monitor*>, use custom allocator instead of new?
// Force redraw when resizing windows.
// Handle negative values when moving windows.
// Add Padding options to screen.
// Fix gap/ratio when resizing since Windows10 use ints for position/size of a window..

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

extern "C" {
    YUBI_API void CALLBACK WinEventCallback(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
    {
    }
}

BOOL CALLBACK EnumWindowCallback(HWND window, LPARAM)
{
    const auto length = GetWindowTextLengthW(window);
    
    // NOTE(Mikyan): We don't want to stop until we've checked all
    // windows.
    if (!IsWindowVisible(window) || length == 0)
        return TRUE;
    
    Window* w = (Window*)VirtualAlloc(0, sizeof(Window), MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
    
    w->Handle = window;
    w->Title = (WCHAR*)VirtualAlloc(0, sizeof(WCHAR) * (length + 1), MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
    GetWindowTextW(window, w->Title, length+1);
    
    for(const auto& iTitle : IgnoredWindows)
    {
        if (wcscmp(w->Title, iTitle) == 0)
        {
            VirtualFree(w->Title, 0, MEM_RELEASE);
            VirtualFree(w, 0, MEM_RELEASE);
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
    
    auto* nMonitor = (Monitor*)VirtualAlloc(0, sizeof(Monitor), MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
    nMonitor->MonitorIndex = MonitorIdx++;
    nMonitor->MonitorName = (CHAR*)VirtualAlloc(0, sizeof(CHAR) * (CCHDEVICENAME + 1), MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
    memcpy(nMonitor->MonitorName, info.szDevice, CCHDEVICENAME);
    nMonitor->MonitorArea = Rect<i32>(info.rcMonitor.right, info.rcMonitor.bottom);
    nMonitor->WorkingArea = Rect<i32>(info.rcWork.right, info.rcWork.bottom);
    nMonitor->Display = new Screen(nMonitor->WorkingArea.Width, nMonitor->WorkingArea.Height);
    
    Monitors.emplace_back(nMonitor);
    
    return TRUE;
}

BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            EnumDisplayMonitors(nullptr, nullptr, EnumMonitorsCallback, NULL);
            EnumWindows(EnumWindowCallback, NULL);
            
            //for(const auto& monitor : Monitors)
            //monitor->Display->UpdateScreen();
        } break;
        case DLL_PROCESS_DETACH:
        {
            // TODO(Mikyan): Free..?
        } break;
        default:
        break;
    }
    return TRUE;
}