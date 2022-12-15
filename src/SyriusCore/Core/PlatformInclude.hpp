#pragma once

#include <optional>
#include <set>
#include <limits>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <utility>
#include <iostream>
#include <chrono>
#include <random>
#include <map>

#include "../../../../include/SyriusCore/Core/SyriusCoreInclude.hpp"
#include "MemoryAllocationTracker.hpp"

#include <vulkan/vulkan.hpp>

#include "../Dependencies/glad/glad/gl.h"

#if defined(SR_CORE_PLATFORM_WIN64)
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

#elif defined(SR_CORE_PLATFORM_LINUX)

#endif

namespace Syrius{

    inline Time getTimeSinceEpoch(){
        return std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch())
                .count();
    }

    inline Time getTimeSinceEpochMilli(){
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch())
                .count();
    }

}

