#include <MonitorManager.hpp>

MonitorManager::MonitorManager()
{
    NtHandle = LoadLibraryA("ntdll");
    
    if (NtHandle == nullptr)
        throw std::runtime_error("Failed to load ntdll.");
    
    Update();
}

MonitorManager::~MonitorManager()
{
    FreeLibrary(NtHandle);
    Monitors.clear();
}

void MonitorManager::Update()
{
    for (const auto& monitor : Monitors)
        monitor->Display->UpdateScreen();
}

bool MonitorManager::TryRegisterWindow(HWND hwnd)
{
    const auto length = GetWindowTextLengthW(hwnd);
    auto pHandle = GetProcessHandleFromWindow(hwnd);
    auto infoOpt = GetProcessExtendedInformation(pHandle);
    
    if (!Win32Helper::WindowIsValid(hwnd) || Win32Helper::WindowIsCloaked(hwnd) || length == 0 || IsIconic(hwnd))
        return false;
    if (!infoOpt.has_value())
        return false;
    
    auto info = infoOpt.value();
    if (info.IsFrozen || info.IsBackground || info.IsCrossSessionCreate)
        return false;
    
    auto title = static_cast<WCHAR*>(VirtualAlloc(0, sizeof(WCHAR) * (length + 1), MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE));
    GetWindowTextW(hwnd, title, length+1);
    
    if (wcscmp(title, L"Program Manager") == 0)
    {
        VirtualFree(title, 0, MEM_RELEASE);
        return false;
    }
    
    RECT rect = {};
    GetWindowRect(hwnd, &rect);
    
    Window* w = static_cast<Window*>(VirtualAlloc(0, sizeof(Window), MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE));
    w->Handle = hwnd;
    w->IsMinimized = IsIconic(hwnd);
    w->IsMaximized = IsZoomed(hwnd);
    w->Title = std::wstring(title);
    w->Area = Rect<f32>(rect);
    w->FrameBounds = Win32Helper::WindowGetFrameBounds(hwnd);
    w->Offset = Rect<f32> {
        static_cast<f32>(w->Area.X - w->FrameBounds.X),
        static_cast<f32>(w->Area.Y - w->FrameBounds.Y),
        static_cast<f32>(w->Area.Width - w->FrameBounds.Width),
        static_cast<f32>(w->Area.Height - w->FrameBounds.Height)
    };
    
    // NOTE(Mikyan): If we fail to get the corresponding Monitor, we take the nearest
    // from this window if we can, otherwise we return nullptr.
    auto* monitor = GetMonitorFromWindow(w->Handle);
    
    if (monitor != nullptr)
        monitor->Display->AddWindow(w);
    
    return true;
}

bool MonitorManager::TryUnregisterWindow(HWND hwnd)
{
    return false;
}

void MonitorManager::HandleClockwiseRotation()
{
    auto hwnd = GetForegroundWindow();
    
    if (hwnd == nullptr)
        return;
    
    auto* monitor = GetMonitorFromWindow(hwnd);
    auto* node = monitor->Display->Window_GetFromWindowHandle(hwnd);
    
    if (node == nullptr)
        return;
    
    auto* prevNode = monitor->Display->Window_GetPrevLeaf(node);
    
    if (prevNode == nullptr)
        return;
    
    
    monitor->Display->Window_Swap(node, prevNode);
    monitor->Display->UpdateScreen();
}

void MonitorManager::HandleCounterClockwiseRotation()
{
    auto hwnd = GetForegroundWindow();
    
    if (hwnd == nullptr)
        return;
    
    auto* monitor = GetMonitorFromWindow(hwnd);
    auto* node = monitor->Display->Window_GetFromWindowHandle(hwnd);
    
    if (node == nullptr)
        return;
    
    auto* nextNode = monitor->Display->Window_GetNextLeaf(node);
    
    if (nextNode == nullptr)
        return;
    
    monitor->Display->Window_Swap(node, nextNode);
    monitor->Display->UpdateScreen();
}


void MonitorManager::HandleSplitDirection()
{
    auto hwnd = GetForegroundWindow();
    
    if (hwnd == nullptr)
        return;
    
    auto* monitor = GetMonitorFromWindow(hwnd);
    auto* node = monitor->Display->Window_GetFromWindowHandle(hwnd);
    
    if (node == nullptr)
        return;
    
    if (node->Split == SplitMode::NONE)
        return;
    
    if (node->Split == SplitMode::VERTICAL)
    {
        // TODO(Mikyan): Force horizontal split
    }
    else
    {
        // TODO(Mikyan): Force vertical split
    }
}

HANDLE MonitorManager::GetProcessHandleFromWindow(HWND hwnd)
{
    if (hwnd == nullptr)
        return nullptr;
    
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    
    if (pid == 0)
        return nullptr;
    
    return OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
}

std::optional<PROCESS_EXTENDED_BASIC_INFORMATION> MonitorManager::GetProcessExtendedInformation(HANDLE process)
{
    pfnZwQueryInformationProcess ZwQueryInfoProcess = NULL;
    
    if (NtHandle == nullptr)
        return {};
    
    ZwQueryInfoProcess = (pfnZwQueryInformationProcess)GetProcAddress(NtHandle, "ZwQueryInformationProcess");
    
    if (ZwQueryInfoProcess == nullptr)
        return {};
    
    PROCESS_EXTENDED_BASIC_INFORMATION pebi = {};
    
    NTSTATUS status = ZwQueryInfoProcess(process, ProcessBasicInformation, &pebi, sizeof(pebi), nullptr);
    
    if (NT_SUCCESS(status))
    {
        CloseHandle(process);
        return pebi;
    }
    
    CloseHandle(process);
    return {};
    
}

Monitor* MonitorManager::GetMonitorFromWindow(HWND hwnd)
{
    auto win32Monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    
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