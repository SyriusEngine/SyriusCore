#include "CoreLoggerDefs.hpp"

namespace Syrius{

    std::string getMessageSourceString(SR_CORE_MESSAGE_SOURCE source){
        switch(source){
            case SR_CORE_MESSAGE:           return "Core";
            case SR_CORE_MESSAGE_OPENGL:    return "OpenGL";
            case SR_CORE_MESSAGE_VULKAN:    return "Vulkan";
            case SR_CORE_MESSAGE_D3D11:     return "Direct3D 11";
            case SR_CORE_MESSAGE_HRESULT:   return "HRESULT";
            case SR_CORE_MESSAGE_DXGI:      return "DXGI";
            case SR_CORE_MESSAGE_WIN32:     return "Win32";
            case SR_CORE_MESSAGE_X11:       return "X11";
            case SR_CORE_MESSAGE_WAYLAND:   return "Wayland";
            default: return "Unknown";
        }
    }
}