#include <iostream>
#include <Windows.h>
#include <vector>
#include "Window.h"

// TODO(Mikyan): Remove std::vector, use custom allocator instead of new.
std::vector<Window> g_Windows;

BOOL CALLBACK EnumWindowCallback(HWND window, LPARAM)
{
    const auto length = GetWindowTextLengthW(window);
    
    if (!IsWindowVisible(window) || length == 0)
        return TRUE;
    
    Window w;
    
    w.Handle = window;
    w.Title = new WCHAR[length+1];
    GetWindowTextW(window, w.Title, length+1);
    GetWindowRect(window, &w.Rect);
    
    g_Windows.push_back(w);
    
    return TRUE;
}


// NOTE(Mikyan): Just used for testing.
void MoveMultipleWindow()
{
    HDWP p = BeginDeferWindowPos(2);
    
    // Move window using DeferWindowPos()
    //DeferWindowPos(p, g_Windows[0].Handle, HWND_TOP, 0, 0, 500, 500, 0);
    //DeferWindowPos(p, g_Windows[1].Handle, HWND_TOP, 400, 50, 300, 500, 0);
    
    if (!EndDeferWindowPos(p))
    {
        std::cout << "EndDeferWindowPos failed." << std::endl;
    }
}

int main()
{
    EnumWindows(EnumWindowCallback, NULL);
    
    for (const auto& w : g_Windows)
        std::wcout << w.Title << std::endl;
}