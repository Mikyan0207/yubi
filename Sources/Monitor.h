#ifndef _MONITOR_H
#define _MONITOR_H

#include "Types.h"
#include "Rect.h"
#include "Vector.h"
#include "Screen.h"

// NOTE(Mikyan): GetMonitorInfoA can return negative values
// if we're getting information for a secondary monitor.
struct Monitor
{
    CHAR* MonitorName;
    
    HMONITOR Win32Monitor;
    
    Vector2<i32> Position;
    Rect<i32> MonitorArea;
    Rect<i32> WorkingArea;
    
    Screen* Display;
    
    void HandleClockwiseRotation();
    void HandleCounterClockwiseRotation();
    void HandleWindowDestroy(HWND hwnd);
    void HandleSplitDirection();
};

#endif //_MONITOR_H
