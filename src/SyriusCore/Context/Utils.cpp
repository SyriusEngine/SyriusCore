#include "../../../include/SyriusCore/Context/Utils.hpp"

namespace Syrius{

    uint8 getTypeSize(SR_DATA_TYPE type){
        switch (type) {
            case SR_FLOAT32:   return 4;
            case SR_FLOAT64:   return 8;
            case SR_INT32:     return 4;
            case SR_UINT32:    return 4;
            case SR_INT16:     return 2;
            case SR_UINT16:    return 2;
            case SR_INT8:      return 1;
            case SR_UINT8:     return 1;
            case SR_TYPE_NONE: return 0;
            case SR_VOID:      return 0;
        }
        return 0;
    }
    uint8 getTypeSize(SR_SCALAR_TYPE type){
        uint8 ccount = ((type << 4) >> 4);
        ccount++;
        return getTypeSize(static_cast<SR_DATA_TYPE>(type)) * ccount;
    }

    uint8 SR_API getScalarComponentCount(SR_SCALAR_TYPE type){
        uint8 shifted = ((type << 4) >> 4);
        shifted++;
        return shifted;
    }
}