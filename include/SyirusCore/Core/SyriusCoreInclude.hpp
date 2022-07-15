#pragma once

#include "PlatformDetection.hpp"

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <utility>
#include <deque>
#include <thread>
#include <iostream>
#include <chrono>
#include <random>

#if defined(SR_PLATFORM_WIN64)
#include <windows.h>
#include <dshow.h>
#include <commdlg.h>
#include <comdef.h>

#include "../Dependencies/glad/glad/wgl.h"

#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <dxgiformat.h>

#elif defined(SR_PLATFORM_UNIX)

#endif

#include "../Dependencies/glad/glad/gl.h"

#include "vulkan/vulkan.hpp"

namespace Syrius{

    typedef enum SR_SUPPORTED_API{
        SR_API_NONE     = 0x00,
        SR_API_OPENGL   = 0x01,
        SR_API_VULKAN   = 0x02,

#if defined(SR_PLATFORM_WIN64)
        SR_API_D3D11    = 0x03,
        SR_API_D3D12    = 0x04,
#endif

    } SR_SUPPORTED_API;

    typedef uint8_t uint8;
    typedef int8_t int8;
    typedef uint16_t uint16;
    typedef int16_t int16;
    typedef uint32_t uint32;
    typedef int32_t int32;
    typedef uint64_t uint64;
    typedef int64_t int64;
    typedef unsigned char ubyte;
    typedef char byte;

    typedef size_t Size;
    typedef uint64 Index;
    typedef uint64 Time;

}

