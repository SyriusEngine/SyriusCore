#include "../../include/SyriusCore/SyriusCore.hpp"
#include "Utils/CoreLogger.hpp"

#include "Window/SyriusWindowWin32Impl.hpp"
#include "Window/SyriusWindowX11Impl.hpp"

namespace Syrius {

    UP<SyriusWindow> createWindow(const WindowDesc& windowDesc){
#if defined(SR_PLATFORM_WIN64)
        auto ptr = new SyriusWindowWin32Impl(windowDesc);
        return UP<SyriusWindow>(ptr);
#elif defined(SR_PLATFORM_LINUX)
        auto ptr = new SyriusWindowX11Impl(windowDesc);
        return Resource<SyriusWindow>(ptr);
#endif
    }
}