#include <WinEvents.hpp>
#include <Windows.h>
#include <iostream>
#include <array>
#include <vector>

LRESULT WndProc(HWND window, uint32_t message, WPARAM wparam, LPARAM lparam)
{
    if (message == WM_PRIV_WINDOWCREATED)
        std::cout << "New window created." << std::endl;

    return 0;
}

bool CALLBACK EnumMonitorsCallback(HMONITOR monitor, HDC, LPRECT, LPARAM)
{
    
}

void CALLBACK WinHookProc(HWINEVENTHOOK winEventHook, DWORD event, HWND window, LONG object, LONG child, DWORD eventThread, DWORD eventTime)
{
    if (event == EVENT_OBJECT_LOCATIONCHANGE)
        std::cout<<"Location Change"<<std::endl;

    if (event == EVENT_SYSTEM_MOVESIZESTART)
        std::cout<<"Move Size Start"<<std::endl;

    if (event == EVENT_SYSTEM_MOVESIZEEND)
        std::cout<<"Move Size End"<<std::endl;

    if (event == EVENT_OBJECT_SHOW)
        std::cout<<"Window visible"<<std::endl;

    if (event == EVENT_OBJECT_CREATE)
        std::cout<<"New window created"<<std::endl;
    if (event == EVENT_OBJECT_UNCLOAKED)
        std::cout<<"Window uncloacked"<<std::endl;
}

std::vector<HWINEVENTHOOK> Hooks = {};

void InitHooks()
{   
    std::array<DWORD, 6> Events = {
        EVENT_SYSTEM_MOVESIZESTART,
        EVENT_SYSTEM_MOVESIZEEND,
        EVENT_OBJECT_NAMECHANGE,
        EVENT_OBJECT_UNCLOAKED,
        EVENT_OBJECT_SHOW,
        EVENT_OBJECT_CREATE
    };

    for (const auto& event : Events)
    {
        auto hook = SetWinEventHook(event, event, nullptr, WinHookProc, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);

        if (hook)
            Hooks.emplace_back(hook);
        else
            std::cout<<"Hook failed."<<std::endl;
    }
}

int main()
{
    InitHooks();

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

