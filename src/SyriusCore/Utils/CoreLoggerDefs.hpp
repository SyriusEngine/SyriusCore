#pragma once

#include "PlatformInclude.hpp"

typedef enum SR_CORE_MESSAGE_SOURCE{
    SR_CORE_MESSAGE         = 0x00,

    // Graphics APIs
    SR_CORE_MESSAGE_OPENGL  = 0x01,
    SR_CORE_MESSAGE_VULKAN  = 0x02,
    SR_CORE_MESSAGE_D3D11   = 0x03,

    // Win32 API
    SR_CORE_MESSAGE_HRESULT = 0x10,
    SR_CORE_MESSAGE_DXGI    = 0x11,
    SR_CORE_MESSAGE_WIN32   = 0x12,

    // Unix API
    SR_CORE_MESSAGE_X11     = 0x20,
    SR_CORE_MESSAGE_WAYLAND = 0x21,
} SR_CORE_MESSAGE_SOURCE;

namespace Syrius{

    std::string getMessageSourceString(SR_CORE_MESSAGE_SOURCE source);

}