#include "../../../include/SyriusCore/Context/Utils.hpp"
#include "../Core/DebugMacros.hpp"

namespace Syrius{

    uint8 getTypeSize(SR_DATA_TYPE type){
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
                SR_CORE_WARNING("Unknown type given to converter");
                return 0;
            }
        }
    }
    uint8 getTypeSize(SR_SCALAR_TYPE type){
        uint8 trunc = type << 4;
        uint8 shifted = (trunc >> 4);
        shifted++;
        return getTypeSize(static_cast<SR_DATA_TYPE>(static_cast<SR_DATA_TYPE>((type >> 4) << 4))) * shifted;
    }

    uint8 getScalarComponentCount(SR_SCALAR_TYPE type){
        uint8 trunc = type << 4;
        uint8 shifted = (trunc >> 4);
        shifted++;
        return shifted;
    }

    SR_DATA_TYPE getScalarDataType(SR_SCALAR_TYPE type){
        return static_cast<SR_DATA_TYPE>((type >> 4) << 4);
    }

    SR_TEXTURE_FORMAT getTextureFormat(SR_TEXTURE_DATA_FORMAT format){
        return static_cast<SR_TEXTURE_FORMAT>((format >> 4) << 4);
    }

    uint8 getTextureChannelCount(SR_TEXTURE_FORMAT format){
        switch (format) {
            case SR_TEXTURE_FORMAT_R:
                return 1;
            case SR_TEXTURE_FORMAT_RG:
                return 2;
            case SR_TEXTURE_FORMAT_RGB:
                return 3;
            case SR_TEXTURE_FORMAT_RGBA:
                return 4;
            case SR_TEXTURE_FORMAT_BGR:
                return 3;
            case SR_TEXTURE_FORMAT_BGRA:
                return 4;
            default: {
                SR_CORE_WARNING("Unknown type given to converter");
                return 0;
            }
        }
    }
}