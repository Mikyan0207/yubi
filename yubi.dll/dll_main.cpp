#include <Windows.h>
#include <psapi.h>

#include <vector>
#include <stdexcept>

#include <Monitor.h>
#include <nt.hpp>
#include <Keys.h>
#include <Screen.hpp>
#include <Window.h>
#include <Win32Helper.h>

// TODO(Mikyan): Remove std::vector<Monitor*>, use custom allocator instead of new?
// Chromium based browsers are broken?
// Handle negative values when moving windows.
// Add Padding options to screen.
// Get window borders size and calculate offset.
// Fix gap/ratio when resizing since Windows10 use ints for position/size of a window..
// /!\ ShellHostExperience.exe is detected as a Window and it's annoying af.
// Limit max depth -> 3 or 4 ?

global std::vector<Monitor*> Monitors;

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

internal HANDLE GetProcessHandleFromWindow(HWND window)
{
    if (window == nullptr)
        return nullptr;
    
    DWORD pid = 0;
    GetWindowThreadProcessId(window, &pid);
    
    if (pid == 0)
        return nullptr;
    
    return OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
}

internal PROCESS_EXTENDED_BASIC_INFORMATION GetProcessExtendedInformation(HANDLE process)
{
    pfnZwQueryInformationProcess ZwQueryInfoProcess = NULL;
    
    local_persist HMODULE dll = LoadLibraryA("ntdll");
    
    if (dll == nullptr)
        throw std::runtime_error("Failed to load ntdll.");
    
    ZwQueryInfoProcess = (pfnZwQueryInformationProcess)GetProcAddress(dll, "ZwQueryInformationProcess");
    
    if (ZwQueryInfoProcess == nullptr)
        throw std::runtime_error("Failed to load ZwQueryInformationProcess");
    
    PROCESS_EXTENDED_BASIC_INFORMATION pbei = {};
    
    NTSTATUS status = ZwQueryInfoProcess(process, ProcessBasicInformation, &pbei, sizeof(pbei), nullptr);
    
    if (NT_SUCCESS(status))
        return pbei;
    
    throw std::runtime_error("Failed to query process information");
}


internal bool TryRegisterWindow(HWND window)
{
    const auto length = GetWindowTextLengthW(window);
    auto pHandle = GetProcessHandleFromWindow(window);
    auto info = GetProcessExtendedInformation(pHandle);
    
    if (length == 0 || IsIconic(window))
        return false;
    
    if (!Win32Helper::WindowIsValid(window) || Win32Helper::WindowIsCloaked(window))
        return false;
    
    if (info.IsFrozen || info.IsBackground || info.IsCrossSessionCreate)
        return false;
    
    auto title = static_cast<WCHAR*>(VirtualAlloc(0, sizeof(WCHAR) * (length + 1), MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE));
    GetWindowTextW(window, title, length+1);
    
    if (wcscmp(title, L"Program Manager") == 0)
        return false;
    
    RECT rect = {};
    GetWindowRect(window, &rect);
    
    Window* w = static_cast<Window*>(VirtualAlloc(0, sizeof(Window), MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE));
    w->Handle = window;
    w->IsMinimized = IsIconic(window);
    w->IsMaximized = IsZoomed(window);
    w->Title = std::wstring(title);
    w->Area = Rect<f32>(rect);
    w->FrameBounds = Win32Helper::WindowGetFrameBounds(window);
    w->Offset = Rect<f32> {
        static_cast<f32>(w->Area.X - w->FrameBounds.X),
        static_cast<f32>(w->Area.Y - w->FrameBounds.Y),
        static_cast<f32>(w->Area.Width - w->FrameBounds.Width),
        static_cast<f32>(w->Area.Height - w->FrameBounds.Height)
    };
    
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