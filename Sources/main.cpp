#include <Windows.h>
#include "Monitor.h"
#include "yubi.h"
#include <iostream>

// NOTE(Mikyan): For global keybinds/shortcuts see RegisterHotKey / UnregisterHotKey.
// NOTE(Mikyan): Retreive focused window handle with GetFocus.

int main()
{
    auto handle = LoadLibraryA("yubi");
    
    // NOTE(Mikyan): We could use a out of context WinEvent, but InContext should be faster.
    // 0x8000 - 0x80FF
    HWINEVENTHOOK hook = SetWinEventHook(EVENT_OBJECT_CREATE, EVENT_OBJECT_END, handle,
                                         (WINEVENTPROC)GetProcAddress(handle, "WinEventCallback"), 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
    
    // NOTE(Mikyan): This is required for receiving events.
    // But that's not a problem since we need a main loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    UnhookWinEvent(hook);
    FreeLibrary(handle);
}