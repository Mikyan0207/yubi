#ifndef _MONITOR_H
#define _MONITOR_H

#include "Types.h"
#include "Rect.h"
#include "Screen.h"

// NOTE(Mikyan): GetMonitorInfoA can return negative values
// if we're getting information for a secondary monitor.
struct Monitor
{
    u32 MonitorIndex;
    CHAR* MonitorName;
    HMONITOR Win32Monitor;
    Rect<i32> MonitorArea;
    Rect<i32> WorkingArea;
    Screen* Display;
};

#endif //_MONITOR_H
