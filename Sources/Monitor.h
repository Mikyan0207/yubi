#ifndef _MONITOR_H
#define _MONITOR_H

#include "Types.h"
#include "Rect.h"
#include "Vector.h"
#include "Screen.h"

enum class SwapDirection
{
    CW, // Clockwise
    CCW // Counter Clockwise
};

// NOTE(Mikyan): GetMonitorInfoA can return negative values
// if we're getting information for a secondary monitor.
struct Monitor
{
    u32 MonitorIndex;
    CHAR* MonitorName;
    
    HMONITOR Win32Monitor;
    
    Vector2<i32> Position;
    Rect<i32> MonitorArea;
    Rect<i32> WorkingArea;
    
    Screen* Display;
    
    void HandleWindowSwap(SwapDirection direction);
    void HandleWindowDestroy(HWND hwnd);
};

#endif //_MONITOR_H
