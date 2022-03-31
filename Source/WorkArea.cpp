#include <WorkArea.hpp>
#include <iostream>

namespace yubi
{
    bool WorkArea::Initialize(HMONITOR monitor)
    {
        MONITORINFOEX info {};
        info.cbSize = sizeof(MONITORINFOEX);

        if (!GetMonitorInfoA(monitor, &info))
        {
            return false;
        }

        m_Area = Rect<int32_t>(info.rcWork);
        std::cout<<m_Area.Width<< " " << m_Area.Height<<std::endl; 

        return true;
    }
}
