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

    uint8 getTypeSize(SR_TYPE type){
        switch (type) {
            case SR_TYPE_NONE:  return 0;
            case SR_VOID:       return 0;
            case SR_UINT8:      return 1;
            case SR_INT8:       return 1;
            case SR_UINT16:     return 2;
            case SR_INT16:      return 2;
            case SR_UINT32:     return 4;
            case SR_INT32:      return 4;
            case SR_UINT64:     return 8;
            case SR_INT64:      return 8;
            case SR_FLOAT16:    return 2;
            case SR_FLOAT32:    return 4;
            case SR_FLOAT64:    return 8;
            default: {
                SR_CORE_WARNING("Unknown type (%i) given to converter", type);
                return 0;
            }
        }
    }
    uint8 getTypeSize(SR_SCALAR_TYPE type){
        uint8 trunc = type << 4;
        uint8 componentCount = (trunc >> 4) + 1;
        return componentCount * getTypeSize(static_cast<SR_TYPE>((type >> 4) << 4));
    }

    uint8 getScalarComponentCount(SR_SCALAR_TYPE type){
        uint8 trunc = type << 4;
        return (trunc >> 4) + 1;
    }

    SR_TYPE getScalarType(SR_SCALAR_TYPE type){
        return static_cast<SR_TYPE>((type >> 4) << 4);
    }

    uint8 getTextureChannelCount(SR_TEXTURE_FORMAT format){
        return format >> 4;
    }

    SR_TYPE getTextureDataType(SR_TEXTURE_FORMAT format){
        return static_cast<SR_TYPE>(format << 4);
    }

}
