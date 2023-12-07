#pragma once

#include "../Utils/PlatformInclude.hpp"

#include "../../../include/SyriusCore/Window/Window.hpp"
#include "../../../include/SyriusCore/Window/Event/KeyboardEvent.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

#include "../../../include/SyriusCore/Dependencies/imgui/imgui.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


namespace Syrius{

    SR_KEYBOARD_KEY convertVirtualKey(WPARAM key, LPARAM flags);

    DWORD decodeWindowStyle(uint32_t srWinStyle);

    DWORD decodeExtendedWindowStyle(uint32_t srWinStyle);

    std::string wideToNormalString(const wchar_t* source);

    std::wstring stringToWideString(const std::string& source);

}

#endif

