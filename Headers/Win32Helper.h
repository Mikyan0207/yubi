#ifndef _WIN32_HELPER_H
#define _WIN32_HELPER_H

#include <Windows.h>
#include "yubi.h"

class YUBI_API Win32Helper
{
    public:
    static LONG GetWindowStyles(HWND window);
    static LONG GetWindowExStyles(HWND window);
    static bool WindowIsValid(HWND window);
    static bool WinEventWindowIsValid(LONG objectId, LONG childId, HWND hwnd);
};

#endif //_WIN32_HELPER_H
