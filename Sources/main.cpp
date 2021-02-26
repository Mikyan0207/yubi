#include <iostream>
#include <Windows.h>
#include <vector>

// TODO(Mikyan): Remove std::vector, use custom allocator instead of new.
BOOL CALLBACK EnumWindowCallback(HWND window, LPARAM lparam)
{
    const auto length = GetWindowTextLengthW(window);
    
    if (!IsWindowVisible(window) || length == 0)
        return TRUE;
    
    
    WCHAR* title = new WCHAR[length+1];
    GetWindowTextW(window, title, length+1);
    
    std::vector<std::wstring>& titles = *reinterpret_cast<std::vector<std::wstring>*>(lparam);
    
    titles.push_back(title);
    
    return TRUE;
}

int main()
{
    // NOTE(Mikyan): Check EnumWindowCallback TODO.
    std::vector<std::wstring> titles;
    
    EnumWindows(EnumWindowCallback, reinterpret_cast<LPARAM>(&titles));
    
    for (const auto& title : titles)
        std::wcout << title << std::endl;
}