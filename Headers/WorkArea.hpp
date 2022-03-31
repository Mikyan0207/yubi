#pragma once
#include <Common/Rect.hpp>
#include <Windows.h>

namespace yubi
{
    class WorkArea
    {
    public:
        WorkArea() = default;
        WorkArea(WorkArea &&) = default;
        WorkArea(const WorkArea &) = default;
        WorkArea &operator=(WorkArea &&) = default;
        WorkArea &operator=(const WorkArea &) = default;
        ~WorkArea() = default;

    public:
        bool Initialize(HMONITOR monitor);

    private:
        Rect<int32_t> m_Area = {};    
    };
}
