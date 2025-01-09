#include "../../../include/SyriusCore/Utils/SyriusCoreInclude.hpp"
#include "DebugMacros.hpp"

namespace Syrius{

    std::string getAPIName(SR_SUPPORTED_API api) {
        switch (api) {
            case SR_API_NONE:       return "None";
            case SR_API_OPENGL:     return "OpenGL";
            case SR_API_VULKAN:     return "Vulkan";
            case SR_API_D3D11:      return "DirectX 11";
            case SR_API_D3D12:      return "DirectX 12";
            default:                return "Unknown";
        }
    }
}
