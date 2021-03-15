#ifndef _WIN32_HELPER_H
#define _WIN32_HELPER_H

#include <Windows.h>
#include <dwmapi.h>
#include <utility>
#include "Types.h"
#include "yubi.h"
#include "Rect.h"

class YUBI_API Win32Helper
{
    public:
    static LONG GetWindowStyles(HWND window);
    static LONG GetWindowExStyles(HWND window);
    static bool WindowIsValid(HWND window);
    static bool WinEventWindowIsValid(LONG objectId, LONG childId, HWND hwnd);
    static bool WindowIsCloaked(HWND window);
    static Rect<i32> WindowGetFrameBounds(HWND window);
};

#endif //_WIN32_HELPER_H
