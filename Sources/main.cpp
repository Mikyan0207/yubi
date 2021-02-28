#include <Windows.h>
#include "Monitor.h"
#include "yubi.h"
#include <iostream>

// NOTE(Mikyan): Callback defined in yubi.dll
//LRESULT CALLBACK ShellProcCallback(int, WPARAM, LPARAM);

int main()
{
    auto handle = LoadLibraryA("yubi");
    
    // NOTE(Mikyan): We could use a out of context WinEvent, but InContext should be faster.
    // 0x8000 - 0x80FF
    HWINEVENTHOOK hook = SetWinEventHook(EVENT_OBJECT_CREATE, EVENT_OBJECT_END, handle,
                                         (WINEVENTPROC)GetProcAddress(handle, "WinEventCallback"), 0, 0, WINEVENT_INCONTEXT | WINEVENT_SKIPOWNPROCESS);
    
    // NOTE(Mikyan): This is required for receiving events.
    // But that's not a problem since we need a main loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    UnhookWinEvent(hook);
}