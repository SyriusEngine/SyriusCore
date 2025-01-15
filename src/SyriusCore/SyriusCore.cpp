#include "../../include/SyriusCore/SyriusCore.hpp"
#include "Utils/CoreLogger.hpp"

#include "Window/SyriusWindowWin32Impl.hpp"
#include "Window/SyriusWindowGlfw3Impl.hpp"

namespace Syrius {

    UP<SyriusWindow> createWindow(const WindowDesc& windowDesc){
#if defined(SR_PLATFORM_WIN64)
        auto ptr = new SyriusWindowWin32Impl(windowDesc);
        return UP<SyriusWindow>(ptr);
#else
        // We fall back to the GLFW3 implementation
        return createUP<SyriusWindowGlfw3Impl>(windowDesc);
#endif
    }
}