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

#include "../../../include/SyriusCore/Utils/SyriusCoreInclude.hpp"
#include "MemoryAllocationTracker.hpp"

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
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#include <cstring>

#endif

namespace Syrius{

}

