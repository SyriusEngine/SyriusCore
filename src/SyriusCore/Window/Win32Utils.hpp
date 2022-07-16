#pragma once

#include "../Core/PlatformInclude.hpp"

#include "../../../../include/SyriusCore/Window/SyriusWindow.hpp"
#include "../../../../include/SyriusCore/Window/Event/KeyboardEvent.hpp"

#if defined(SR_PLATFORM_WIN64)

#include "../../../include/SyriusCore/Dependencies/imgui/imgui.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


namespace Syrius{

    static const wchar_t* s_SyriusWindowClass = L"SYRIUS_CORE_API";

    void SR_API setProcessDpiAware();

    SR_KEYBOARD_KEY SR_API convertVirtualKey(WPARAM key, LPARAM flags);

    DWORD SR_API decodeWindowStyle(uint32_t srWinStyle);

    DWORD SR_API decodeExtendedWindowStyle(uint32_t srWinStyle);

    std::string SR_API wideToNormalString(const wchar_t* source);

    std::wstring SR_API stringToWideString(const std::string& source);

}

#endif

