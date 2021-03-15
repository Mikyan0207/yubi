#include <vector>
#include <iostream>
#include <Windows.h>
#include <psapi.h>

#include "Window.h"
#include "Screen.h"
#include "Monitor.h"
#include "Keys.h"
#include "Win32Helper.h"

// NOTE(Mikyan): Somehow, I can't get the RECT for certain window like
// Chromium based browsers are broken?

// TODO(Mikyan): Remove std::vector<Monitor*>, use custom allocator instead of new?
// Force redraw when resizing windows. -> Currently done by focusing each window when resizing
// Handle negative values when moving windows.
// Add Padding options to screen.
// Get window borders size and calculate offset.
// Fix gap/ratio when resizing since Windows10 use ints for position/size of a window..
// /!\ ShellHostExperience.exe is detected as a Window and it's annoying af.
// /!\ Search if we can override the min size of a window. -> First result says it's not possible
// Limit max depth -> 3 or 4 ?

global std::vector<Monitor*> Monitors;

global WCHAR* IgnoredWindows[] = {
    L"Program Manager",
    L"Microsoft Text Input Application",
};

internal Monitor* GetMonitorFromWindow(HWND window, DWORD flags)
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

internal bool TryRegisterWindow(HWND window)
{
    const auto length = GetWindowTextLengthW(window);
    
    if (!IsWindowVisible(window) || length == 0 || IsIconic(window))
        return false;
    
    // Double check
    // NOTE(Mikyan): Some windows are not detected as valid but should be.
    if (!Win32Helper::WindowIsValid(window))
        return false;
    
    Window* w = static_cast<Window*>(VirtualAlloc(0, sizeof(Window), MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE));
    
    w->Handle = window;
    w->Title = static_cast<WCHAR*>(VirtualAlloc(0, sizeof(WCHAR) * (length + 1), MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE));
    GetWindowTextW(window, w->Title, length+1);
    
    for(const auto& iTitle : IgnoredWindows)
    {
        if (wcscmp(w->Title, iTitle) == 0)
        {
            VirtualFree(w->Title, 0, MEM_RELEASE);
            VirtualFree(w, 0, MEM_RELEASE);
            return false;
        }
    }
    
    // TODO(Mikyan): Get Border/Offset of the window
    // DwmGetWindowAttribute() with DWMWA_EXTENDED_FRAME_BOUNDS
    RECT frameBounds{};
    
    // NOTE(Mikyan): If we fail to get the corresponding Monitor, we take the nearest
    // from this window if we can, otherwise we return nullptr.
    auto* monitor = GetMonitorFromWindow(w->Handle, MONITOR_DEFAULTTONEAREST);
    
    if (monitor != nullptr)
        monitor->Display->AddWindow(w);
    
    return true;
}

// NOTE(Mikyan): These functions are declared extern "C" to avoid name mangling with C++
// since we load them with GetProcAddress.
extern "C" {
    YUBI_API void CALLBACK WinEventCallback(HWINEVENTHOOK hook, DWORD event, HWND window, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
    {
        if (Win32Helper::WinEventWindowIsValid(idObject, idChild, window))
        {
            // TODO(Mikyan): Handle cloaking.
            // TODO(Mikyan): Handle Minimize/Maximize
            switch (event)
            {
                case EVENT_OBJECT_SHOW:
                {
                    printf("OBJ SHOW - NEW WINDOW\n");
                    if (TryRegisterWindow(window))
                    {
                        printf("The window is valid and was added to the tree.\n");
                    }
                    else
                    {
                        printf("Ignoring invalid Window\n");
                    }
                } break;
                case EVENT_OBJECT_DESTROY:
                {
                    printf("WINDOW DESTROYED\n");
                    auto* monitor = GetMonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);
                    
                    if (monitor != nullptr)
                        monitor->HandleWindowDestroy(window);
                } break;
                default:
                break;
            }
        }
    }
    
    YUBI_API void HandleHotKey(LPARAM lParam)
    {
        UINT modifier = LOWORD(lParam);
        UINT key = HIWORD(lParam);
        
        if (modifier == MOD_ALT && key == KEY_K)
        {
            printf("Swap windows. CW\n");
            auto hwnd = GetForegroundWindow();
            
            auto* monitor = GetMonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
            
            if (monitor != nullptr)
            {
                monitor->HandleClockwiseRotation();
            }
        }
        
        if (modifier == MOD_ALT && key == KEY_L)
        {
            printf("Swap windows. CCW\n");
            auto hwnd = GetForegroundWindow();
            
            auto* monitor = GetMonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
            
            if (monitor != nullptr)
            {
                monitor->HandleCounterClockwiseRotation();
            }
        }
        
        if (modifier == MOD_ALT && key == KEY_O)
        {
            printf("Change split direction");
            
            auto hwnd = GetForegroundWindow();
            auto* monitor = GetMonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
            
            if (monitor == nullptr)
            {
                monitor->HandleSplitDirection();
            }
        }
    }
}

BOOL CALLBACK EnumWindowCallback(HWND window, LPARAM)
{
    TryRegisterWindow(window);
    
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
    nMonitor->MonitorName = (CHAR*)VirtualAlloc(0, sizeof(CHAR) * (CCHDEVICENAME + 1), MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
    memcpy(nMonitor->MonitorName, info.szDevice, CCHDEVICENAME);
    nMonitor->Position = Vector2<i32>(info.rcWork.left, info.rcWork.top);
    nMonitor->MonitorArea = Rect<i32>(info.rcMonitor.right, info.rcMonitor.bottom);
    nMonitor->WorkingArea = Rect<i32>(info.rcWork.right - info.rcWork.left, info.rcWork.bottom - info.rcWork.top);
    nMonitor->Display = new Screen(nMonitor->WorkingArea.Width, nMonitor->WorkingArea.Height);
    
    Monitors.emplace_back(nMonitor);
    
    printf("Monitor: %s\n", nMonitor->MonitorName);
    
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
            
            for(const auto& monitor : Monitors)
                monitor->Display->UpdateScreen();
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