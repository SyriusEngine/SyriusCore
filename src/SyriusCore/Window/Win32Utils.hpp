#pragma once

#include "../Utils/PlatformInclude.hpp"

#include "../../../include/SyriusCore/Window/SyriusWindow.hpp"
#include "../../../include/SyriusCore/Window/Event/KeyboardEvent.hpp"

#if defined(SR_PLATFORM_WIN64)

#include "../../../include/SyriusCore/Dependencies/imgui/imgui.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


namespace Syrius{

    SR_KEYBOARD_KEY convertVirtualKey(WPARAM key, LPARAM flags);

    DWORD decodeWindowStyle(u32 srWinStyle);

    DWORD decodeExtendedWindowStyle(u32 srWinStyle);

    std::string wideToNormalString(const wchar_t* source);

    std::wstring stringToWideString(const std::string& source);

}

#endif

