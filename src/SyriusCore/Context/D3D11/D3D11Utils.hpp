#pragma once

#include "../../../../include/SyriusCore/Context/Utils.hpp"
#include "../../Utils/PlatformInclude.hpp"
#include "../../Utils/DebugMacros.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    D3D11_USAGE getD3d11BufferType(SR_BUFFER_USAGE type);

    DXGI_FORMAT getD3d11DataType(SR_TYPE dataType);

    DXGI_FORMAT getD3d11ScalarType(SR_SCALAR_TYPE scalarType);

    DXGI_FORMAT getD3d11TextureFormat(SR_TEXTURE_FORMAT format);

    D3D11_PRIMITIVE_TOPOLOGY getD3d11DrawMode(SR_DRAW_TYPE drawMode);

    D3D11_TEXTURE_ADDRESS_MODE getD3d11TextureAddressMode(SR_TEXTURE_ADDRESS_WRAP addressMode);

    D3D11_FILTER getD3d11TextureFilter(SR_TEXTURE_FILTER minFilter, SR_TEXTURE_FILTER magFilter);

    D3D11_COMPARISON_FUNC getD3d11ComparisonFunc(SR_COMPARISON_FUNC func);

    D3D11_STENCIL_OP getD3d11StencilFunc(SR_STENCIL_FUNC func);

}

#endif

