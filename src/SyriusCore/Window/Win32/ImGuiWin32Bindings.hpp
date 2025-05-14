#pragma once

#include "../../Utils/PlatformInclude.hpp"
#include "../../../../include/SyriusCore/Dependencies/imgui/imgui.h"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius {

    struct ImGuiWglData {
        HDC hDC = nullptr;
    };

    bool ImGuiCreateDeviceWGL(HWND hwnd, ImGuiWglData* data);

}

#endif