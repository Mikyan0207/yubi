#include <iostream>
#include <Windows.h>
#include <vector>
#include "Window.h"
#include "StackAllocator.h"
// NOTE(Mikyan): Somehow, I can't get the RECT for certain window like
// Edge for example.

// TODO(Mikyan): Remove std::vector, use custom allocator instead of new.
std::vector<Window> g_Windows;

BOOL CALLBACK EnumWindowCallback(HWND window, LPARAM)
{
    const auto length = GetWindowTextLengthW(window);
    
    // NOTE(Mikyan): We don't want to stop until we've checked all
    // the window.
    if (!IsWindowVisible(window) || length == 0)
        return TRUE;
    
    Window w;
    RECT rect;
    
    w.Handle = window;
    w.Title = new WCHAR[length+1]();
    GetWindowTextW(window, w.Title, length+1);
    GetWindowRect(window, &rect);
    
    w.X = rect.left;
    w.Y = rect.top;
    w.Width = rect.right - rect.left;
    w.Height = rect.bottom - rect.top;
    
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
    
    StackAllocator allocator(10);
    
    for (const auto& w : g_Windows)
    {
        allocator.Free(sizeof(w.Title));
        std::wcout << w.Title << " " << w.Width << " " << w.Height << std::endl;
    }
}