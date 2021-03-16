#include <Windows.h>
#include <psapi.h>

#include <vector>
#include <stdexcept>

#include <Monitor.h>
#include <MonitorManager.hpp>
#include <nt.hpp>
#include <Keys.h>
#include <Screen.hpp>
#include <Window.h>
#include <Win32Helper.h>

// Chromium based browsers are broken?
// Handle negative values when moving windows.
// Add Padding options to screen.
// Fix gap/ratio when resizing since Windows10 use ints for position/size of a window..
// /!\ ShellHostExperience.exe is detected as a Window and it's annoying af.
// Limit max depth -> 3 or 4 ?

global MonitorManager* Manager = nullptr;

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
                    if (Manager->TryRegisterWindow(window))
                        printf("Window added.\n");
                } break;
                case EVENT_OBJECT_DESTROY:
                {
                    if (Manager->TryUnregisterWindow(window))
                        printf("Window removed.\n");
                } break;
                default:
                {
                } break;
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
            Manager->HandleClockwiseRotation();
        }
        
        if (modifier == MOD_ALT && key == KEY_L)
        {
            printf("Swap windows. CCW\n");
            Manager->HandleCounterClockwiseRotation();
        }
        
        if (modifier == MOD_ALT && key == KEY_O)
        {
            printf("Change split direction");
            Manager->HandleSplitDirection();
        }
    }
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
    
    Manager->Monitors.emplace_back(nMonitor);
    
    return TRUE;
}

BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM)
{
    if (!Manager->TryRegisterWindow(hwnd))
    {
#ifdef _DEBUG
        std::cout << "Window ignored." << std::endl;
#endif
    }
    
    return TRUE;
}


BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            Manager = new MonitorManager();
            
            if (!Manager)
                throw std::runtime_error("Failed to allocate memory for the MonitorManager.");
            
            EnumDisplayMonitors(nullptr, nullptr, EnumMonitorsCallback, NULL);
            EnumWindows(EnumWindowsCallback, NULL);
            
            Manager->Update();
        } break;
        case DLL_PROCESS_DETACH:
        {
            delete Manager;
            // TODO(Mikyan): Free..?
        } break;
        default:
        break;
    }
    return TRUE;
}