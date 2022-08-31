#include "D3D11Utils.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    D3D11_USAGE getD3d11BufferType(SR_BUFFER_TYPE type){
        switch (type){
            case SR_BUFFER_DEFAULT:     return D3D11_USAGE_DEFAULT;
            case SR_BUFFER_DYNAMIC:     return D3D11_USAGE_DYNAMIC;
            case SR_BUFFER_STATIC:      return D3D11_USAGE_IMMUTABLE;
            default: {
                SR_CORE_WARNING("Invalid type given to converter, return D3D11_USAGE_DEFAULT");
                return D3D11_USAGE_DEFAULT;
            }
        }
    }

    DXGI_FORMAT getD3d11DataType(SR_DATA_TYPE dataType){
        switch (dataType){
            case SR_UINT8:      return DXGI_FORMAT_R8_UINT;
            case SR_INT8:       return DXGI_FORMAT_R8_SINT;
            case SR_UINT16:     return DXGI_FORMAT_R16_UINT;
            case SR_INT16:      return DXGI_FORMAT_R16_SINT;;
            case SR_UINT32:     return DXGI_FORMAT_R32_UINT;;
            case SR_INT32:      return DXGI_FORMAT_R32_SINT;
            case SR_FLOAT16:    return DXGI_FORMAT_R16_FLOAT;
            case SR_FLOAT32:    return DXGI_FORMAT_R32_FLOAT;;
            default: {
                SR_CORE_WARNING("Invalid type given to converter, return DXGI_FORMAT_R32_FLOAT");
                return DXGI_FORMAT_R32_FLOAT;
            }
        }
    }

    DXGI_FORMAT getD3D11ScalarType(SR_SCALAR_TYPE scalarType){
        switch (scalarType){
            case SR_UINT8_1:        return DXGI_FORMAT_R8_UINT;
            case SR_UINT8_2:        return DXGI_FORMAT_R8G8_UINT;
//            case SR_UINT8_3:        return DXGI_FORMAT_R8G8B8_UINT;
            case SR_UINT8_4:        return DXGI_FORMAT_R8G8B8A8_UINT;
//            case SR_UINT8_3X3:      break;
//            case SR_UINT8_4X4:      break;
            case SR_INT8_1:         return DXGI_FORMAT_R8_SINT;
            case SR_INT8_2:         return DXGI_FORMAT_R8G8_SINT;
//            case SR_INT8_3:         return DXGI_FORMAT_R8G8B8_SINT;
            case SR_INT8_4:         return DXGI_FORMAT_R8G8B8A8_SINT;
//            case SR_INT8_3X3:       break;
//            case SR_INT8_4X4:       break;
            case SR_UINT16_1:       return DXGI_FORMAT_R16_UINT;
            case SR_UINT16_2:       return DXGI_FORMAT_R16G16_UINT;
//            case SR_UINT16_3:       return DXGI_FORMAT_R16G16B16_UINT;
            case SR_UINT16_4:       return DXGI_FORMAT_R16G16B16A16_UINT;
//            case SR_UINT16_3X3:     break;
//            case SR_UINT16_4X4:     break;
            case SR_INT16_1:        return DXGI_FORMAT_R16_SINT;
            case SR_INT16_2:        return DXGI_FORMAT_R16G16_SINT;
//            case SR_INT16_3:        return DXGI_FORMAT_R16G16B16_SINT;
            case SR_INT16_4:        return DXGI_FORMAT_R16G16B16A16_SINT;
//            case SR_INT16_3X3:      break;
//            case SR_INT16_4X4:      break;
            case SR_UINT32_1:       return DXGI_FORMAT_R32_UINT;
            case SR_UINT32_2:       return DXGI_FORMAT_R32G32_UINT;
            case SR_UINT32_3:       return DXGI_FORMAT_R32G32B32_UINT;
            case SR_UINT32_4:       return DXGI_FORMAT_R32G32B32A32_UINT;
//            case SR_UINT32_3X3:     break;
//            case SR_UINT32_4X4:     break;
            case SR_INT32_1:        return DXGI_FORMAT_R32_SINT;
            case SR_INT32_2:        return DXGI_FORMAT_R32G32_SINT;
            case SR_INT32_3:        return DXGI_FORMAT_R32G32B32_SINT;
            case SR_INT32_4:        return DXGI_FORMAT_R32G32B32A32_SINT;
//            case SR_INT32_3X3:      break;
//            case SR_INT32_4X4:      break;
//            case SR_UINT64_1:       break;
//            case SR_UINT64_2:       break;
//            case SR_UINT64_3:       break;
//            case SR_UINT64_4:       break;
//            case SR_UINT64_3X3:     break;
//            case SR_UINT64_4X4:     break;
//            case SR_INT64_1:        break;
//            case SR_INT64_2:        break;
//            case SR_INT64_3:        break;
//            case SR_INT64_4:        break;
//            case SR_INT64_3X3:      break;
//            case SR_INT64_4X4:      break;
            case SR_FLOAT16_1:      return DXGI_FORMAT_R16_FLOAT;
            case SR_FLOAT16_2:      return DXGI_FORMAT_R16G16_FLOAT;
//            case SR_FLOAT16_3:      return DXGI_FORMAT_R16G16B16_FLOAT;
            case SR_FLOAT16_4:      return DXGI_FORMAT_R16G16B16A16_FLOAT;
//            case SR_FLOAT16_3X3:    break;
//            case SR_FLOAT16_4X4:    break;
            case SR_FLOAT32_1:      return DXGI_FORMAT_R32_FLOAT;
            case SR_FLOAT32_2:      return DXGI_FORMAT_R32G32_FLOAT;
            case SR_FLOAT32_3:      return DXGI_FORMAT_R32G32B32_FLOAT;
            case SR_FLOAT32_4:      return DXGI_FORMAT_R32G32B32A32_FLOAT;
//            case SR_FLOAT32_3X3:    break;
//            case SR_FLOAT32_4X4:    break;
//            case SR_FLOAT64_1:      break;
//            case SR_FLOAT64_2:      break;
//            case SR_FLOAT64_3:      break;
//            case SR_FLOAT64_4:      break;
//            case SR_FLOAT64_3X3:    break;
//            case SR_FLOAT64_4X4:    break;
            default:{
                SR_CORE_WARNING("Invalid type given to converter, note that D3D11 does not support most 3-component vectors or 64bit types");
                return DXGI_FORMAT_UNKNOWN;
            }
        }
    }

    D3D11_PRIMITIVE_TOPOLOGY getD3d11DrawMode(SR_DRAW_TYPE drawMode){
        switch (drawMode){
            case SR_DRAW_POINTS:            return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
            case SR_DRAW_LINES:             return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
            case SR_DRAW_LINES_STRIP:       return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
            case SR_DRAW_TRIANGLES:         return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            case SR_DRAW_TRIANGLE_STRIP:    return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
            default: {
                SR_CORE_WARNING("Invalid type given to converter, return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST");
                return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            }
        }
    }

}

#endif
