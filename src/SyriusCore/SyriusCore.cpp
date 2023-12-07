#include "../../include/SyriusCore/SyriusCore.hpp"
#include "Utils/MemoryAllocationTracker.hpp"
#include "Utils/DebugMessageHandler.hpp"

#include "Window/SyriusWindowWin32Impl.hpp"
#include "Window/SyriusWindowX11Impl.hpp"

namespace Syrius {

    Resource<SyriusWindow> createWindow(const WindowDesc& windowDesc){
#if defined(SR_CORE_PLATFORM_WIN64)
        auto ptr = new SyriusWindowWin32Impl(windowDesc);
        return Resource<SyriusWindow>(ptr);
#elif defined(SR_CORE_PLATFORM_LINUX)
        auto ptr = new SyriusWindowX11Impl(windowDesc);
        return Resource<SyriusWindow>(ptr);
#endif
    }

    void setDebugMessageCallback(handleDebugMessageFunc callback){
        DebugMessageHandler::setDebugMessageHandler(callback);
    }

    Size getAllocatedMemory(){
        return MemoryAllocationTracker::getTotalAllocated();
    }

    Size getFreedMemory(){
        return MemoryAllocationTracker::getTotalFreed();
    }

    Size getMemoryUsage(){
        return MemoryAllocationTracker::getUsage();
    }

}