#include <WinEvents.hpp>
#include <mutex>
#include <Windows.h>

uint32_t WM_PRIV_MOVESIZESTART;
uint32_t WM_PRIV_MOVESIZEEND;
uint32_t WM_PRIV_LOCATIONCHANGE;
uint32_t WM_PRIV_NAMECHANGE;
uint32_t WM_PRIV_WINDOWCREATED;
uint32_t WM_PRIV_VD_INIT;
uint32_t WM_PRIV_VD_SWITCH;
uint32_t WM_PRIV_VD_UPDATE;
uint32_t WM_PRIV_EDITOR;
uint32_t WM_PRIV_LAYOUT_HOTKEYS_FILE_UPDATE;
uint32_t WM_PRIV_LAYOUT_TEMPLATES_FILE_UPDATE;
uint32_t WM_PRIV_CUSTOM_LAYOUTS_FILE_UPDATE;
uint32_t WM_PRIV_APPLIED_LAYOUTS_FILE_UPDATE;
uint32_t WM_PRIV_SNAP_HOTKEY;
uint32_t WM_PRIV_QUICK_LAYOUT_KEY;
uint32_t WM_PRIV_SETTINGS_CHANGED;

std::once_flag init_flag;

void InitializeWinhookEventIds()
{
    std::call_once(init_flag, [&]
    {
        WM_PRIV_MOVESIZESTART = RegisterWindowMessage("{f48def23-df42-4c0f-a13d-3eb4a9e204d4}");
        WM_PRIV_MOVESIZEEND = RegisterWindowMessage("{805d643c-804d-4728-b533-907d760ebaf0}");
        WM_PRIV_LOCATIONCHANGE = RegisterWindowMessage("{d56c5ee7-58e5-481c-8c4f-8844cf4d0347}");
        WM_PRIV_NAMECHANGE = RegisterWindowMessage("{b7b30c61-bfa0-4d95-bcde-fc4f2cbf6d76}");
        WM_PRIV_WINDOWCREATED = RegisterWindowMessage("{bdb10669-75da-480a-9ec4-eeebf09a02d7}");
        WM_PRIV_VD_INIT = RegisterWindowMessage("{469818a8-00fa-4069-b867-a1da484fcd9a}");
        WM_PRIV_VD_SWITCH = RegisterWindowMessage("{128c2cb0-6bdf-493e-abbe-f8705e04aa95}");
        WM_PRIV_VD_UPDATE = RegisterWindowMessage("{b8b72b46-f42f-4c26-9e20-29336cf2f22e}");
        WM_PRIV_EDITOR = RegisterWindowMessage("{87543824-7080-4e91-9d9c-0404642fc7b6}");
        WM_PRIV_LAYOUT_HOTKEYS_FILE_UPDATE = RegisterWindowMessage("{07229b7e-4f22-4357-b136-33c289be2295}");
        WM_PRIV_LAYOUT_TEMPLATES_FILE_UPDATE = RegisterWindowMessage("{4686f019-5d3d-4c5c-9051-b7cbbccca77d}");
        WM_PRIV_CUSTOM_LAYOUTS_FILE_UPDATE = RegisterWindowMessage("{0972787e-cdab-4e16-b228-91acdc38f40f}");
        WM_PRIV_APPLIED_LAYOUTS_FILE_UPDATE = RegisterWindowMessage("{2ef2c8a7-e0d5-4f31-9ede-52aade2d284d}");
        WM_PRIV_SNAP_HOTKEY = RegisterWindowMessage("{72f4fd8e-23f1-43ab-bbbc-029363df9a84}");
        WM_PRIV_QUICK_LAYOUT_KEY = RegisterWindowMessage("{15baab3d-c67b-4a15-aFF0-13610e05e947}");
        WM_PRIV_SETTINGS_CHANGED = RegisterWindowMessage("{89ca3Daa-bf2d-4e73-9f3f-c60716364e27}");
    });
}
