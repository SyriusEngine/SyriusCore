#pragma once

#include "../../../../include/SyriusCore/Context/Utils.hpp"
#include "../../Core/PlatformInclude.hpp"
#include "../../Core/DebugMacros.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    D3D11_USAGE getD3d11BufferType(SR_BUFFER_TYPE type);

    DXGI_FORMAT getD3d11DataType(SR_DATA_TYPE dataType);

    DXGI_FORMAT getD3d11ScalarType(SR_SCALAR_TYPE scalarType);

    D3D11_PRIMITIVE_TOPOLOGY getD3d11DrawMode(SR_DRAW_TYPE drawMode);

    D3D11_TEXTURE_ADDRESS_MODE getD3d11TextureAddressMode(SR_TEXTURE_ADDRESS_WRAP addressMode);

    D3D11_FILTER getD3d11TextureFilter(SR_TEXTURE_FILTER minFilter, SR_TEXTURE_FILTER magFilter);

}

#endif

