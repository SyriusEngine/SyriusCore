#pragma once

#include <vulkan/vulkan.hpp>

#if defined(SR_PLATFORM_WIN64)
#include <windows.h>
#include <dshow.h>
#include <commdlg.h>
#include <comdef.h>

#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <dxgiformat.h>

#elif defined(SR_PLATFORM_UNIX)

#endif

