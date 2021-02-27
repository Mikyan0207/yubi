#ifndef _MONITOR_H
#define _MONITOR_H

#include "Types.h"
#include "Rect.h"

// NOTE(Mikyan): GetMonitorInfoA can return negative values
// if we're getting information for a secondary monitor.
struct Monitor
{
    u32 MonitorIndex;
    Rect<i32> MonitorArea;
    Rect<i32> WorkingArea;
};

#endif //_MONITOR_H
