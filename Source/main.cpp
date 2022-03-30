#include <WinEvents.hpp>
#include <Windows.h>
#include <iostream>

LRESULT WndProc(HWND window, uint32_t message, WPARAM wparam, LPARAM lparam)
{
    if (message == WM_PRIV_WINDOWCREATED)
        std::cout << "New window created." << std::endl;

    return 0;
}

int main()
{
    return 0;
}

