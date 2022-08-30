#include "D3D11Utils.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    D3D11_USAGE getD3d11BufferType(SR_BUFFER_TYPE type){
        switch (type){
            case SR_BUFFER_DEFAULT:
                return D3D11_USAGE_DEFAULT;
            case SR_BUFFER_DYNAMIC:
                return D3D11_USAGE_DYNAMIC;
            case SR_BUFFER_STATIC:
                return D3D11_USAGE_IMMUTABLE;
            default: {
                SR_CORE_WARNING("Invalid type given to converter, return D3D11_USAGE_DEFAULT");
                return D3D11_USAGE_DEFAULT;
            }
        }
    }

    DXGI_FORMAT getD3d11DataType(SR_DATA_TYPE dataType){
        switch (dataType){
            case SR_UINT8:
                return DXGI_FORMAT_R8_UINT;
            case SR_INT8:
                return DXGI_FORMAT_R8_SINT;
            case SR_UINT16:
                return DXGI_FORMAT_R16_UINT;
            case SR_INT16:
                return DXGI_FORMAT_R16_SINT;;
            case SR_UINT32:
                return DXGI_FORMAT_R32_UINT;;
            case SR_INT32:
                return DXGI_FORMAT_R32_SINT;
            case SR_FLOAT16:
                return DXGI_FORMAT_R16_FLOAT;
            case SR_FLOAT32:
                return DXGI_FORMAT_R32_FLOAT;;
            default: {
                SR_CORE_WARNING("Invalid type given to converter, return DXGI_FORMAT_R32_FLOAT");
                return DXGI_FORMAT_R32_FLOAT;
            }
        }
    }

}

#endif
