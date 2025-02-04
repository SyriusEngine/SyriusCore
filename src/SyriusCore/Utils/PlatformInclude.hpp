#pragma once

#include "../../../include/SyriusCore/Utils/SyriusCoreInclude.hpp"
#include <SyriusUtils/SyriusUtils.hpp>

#include "../Dependencies/glad/glad/gl.h"

#if defined(SR_PLATFORM_WIN64)
#if !defined(NOMINMAX)
    #define NOMINMAX
#endif
    #include <windows.h>
    #include <dshow.h>
    #include <commdlg.h>
    #include <comdef.h>

    #include <d3d11.h>
    #include <dxgi.h>
    #include <d3dcompiler.h>
    #include <dxgiformat.h>
    #include <dxgidebug.h>

#include "../Dependencies/glad/glad/wgl.h"

#else
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <cstring>

#endif

namespace Syrius{

}

