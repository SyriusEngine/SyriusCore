#pragma once

#include <optional>
#include <set>

#include "../../../../include/SyriusCore/Core/SyriusCoreInclude.hpp"

#include <vulkan/vulkan.hpp>

#include "../Dependencies/glad/glad/gl.h"

#if defined(SR_PLATFORM_WIN64)
#include <windows.h>
#include <dshow.h>
#include <commdlg.h>
#include <comdef.h>

#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <dxgiformat.h>

#include "../Dependencies/glad/glad/wgl.h"

#elif defined(SR_PLATFORM_UNIX)

#endif

