#pragma once

#include <Windows.h>

#include <memory>
#include <optional>
#include <stdexcept>
#include <vector>

#include <Monitor.h>
#include <nt.hpp>
#include <Rect.h>
#include <Types.h>
#include <yubi.h>
#include <Win32Helper.h>

class YUBI_API MonitorManager
{
    public:
    MonitorManager();
    MonitorManager(const MonitorManager&) = delete;
    MonitorManager(MonitorManager&&) noexcept = delete;
    ~MonitorManager();
    
    public:
    MonitorManager& operator=(const MonitorManager&) = delete;
    MonitorManager& operator=(MonitorManager&&) noexcept = delete;
    
    public:
    void Update();
    bool TryRegisterWindow(HWND hwnd);
    bool TryUnregisterWindow(HWND hwnd);
    
    void HandleClockwiseRotation();
    void HandleCounterClockwiseRotation();
    void HandleSplitDirection();
    
    private:
    [[nodiscard]] HANDLE GetProcessHandleFromWindow(HWND hwnd);
    [[nodiscard]] std::optional<PROCESS_EXTENDED_BASIC_INFORMATION> GetProcessExtendedInformation(HANDLE process);
    [[nodiscard]] Monitor* GetMonitorFromWindow(HWND hwnd);
    
    public:
    HMODULE NtHandle;
    std::vector<Monitor*> Monitors;
};