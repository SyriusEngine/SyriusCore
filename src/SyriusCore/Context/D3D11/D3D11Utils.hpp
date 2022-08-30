#pragma once

#include "../../../../include/SyriusCore/Context/Utils.hpp"
#include "../../Core/PlatformInclude.hpp"
#include "../../Core/DebugMacros.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    D3D11_USAGE getD3d11BufferType(SR_BUFFER_TYPE type);

    DXGI_FORMAT getD3d11DataType(SR_DATA_TYPE dataType);

}

#endif

