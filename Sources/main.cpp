#include <Windows.h>
#include <iostream>
#include "Monitor.h"
#include "Keys.h"
#include "yubi.h"

typedef void (*HOTKEY_PTR)(LPARAM);
// NOTE(Mikyan): For global keybinds/shortcuts see RegisterHotKey / UnregisterHotKey.
// NOTE(Mikyan): Retreive focused window handle with GetFocus.

internal void RegisterHotKeys(HWND hwnd)
{
    RegisterHotKey(NULL, 1, MOD_ALT | MOD_NOREPEAT, KEY_K);
    RegisterHotKey(NULL, 2, MOD_ALT | MOD_NOREPEAT, KEY_L);
}

internal void UnregisterHotKeys(HWND hwnd)
{
    UnregisterHotKey(NULL, 1);
    UnregisterHotKey(NULL, 2);
}

int main()
{
    auto selfHwnd = GetConsoleWindow();
    auto handle = LoadLibraryA("yubi");
    
    RegisterHotKeys(selfHwnd);
    
    HOTKEY_PTR HandleHotKey = (HOTKEY_PTR)GetProcAddress(handle, "HandleHotKey");
    
    // EVENT_OBJECT_CREATE  0x8000
    // EVENT_OBEJCT_END     0x80FF
    HWINEVENTHOOK hook = SetWinEventHook(EVENT_OBJECT_CREATE, EVENT_OBJECT_END, handle,
                                         (WINEVENTPROC)GetProcAddress(handle, "WinEventCallback"), 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
    
    // NOTE(Mikyan): This is required for receiving events.
    // But that's not a problem since we need a main loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        if (msg.message == WM_HOTKEY)
        {
            HandleHotKey(msg.lParam);
        }
        
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    UnregisterHotKeys(selfHwnd);
    UnhookWinEvent(hook);
    FreeLibrary(handle);
}