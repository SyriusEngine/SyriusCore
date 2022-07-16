#include "CoreCommand.hpp"
#include "PlatformAPIWin32Impl.hpp"

namespace Syrius{

    Time CoreCommand::m_StartupTime = 0;
    uint32 CoreCommand::m_GladInstances = 0;
    uint32 CoreCommand::m_CoreCommandInstances = 0;
    PlatformAPI* CoreCommand::m_PlatformAPI = nullptr;


    void CoreCommand::init() {
        if (!m_CoreCommandInstances){
            m_StartupTime = getTimeSinceEpochMilli();

            SR_CORE_MESSAGE("Initializing Syrius")

#if defined(SR_PLATFORM_WIN64)
            m_PlatformAPI = new PlatformAPIWin32Impl();
#endif

        }
        m_CoreCommandInstances++;

        SR_CORE_POSTCONDITION(m_PlatformAPI != nullptr, "Failed to create platform API")
    }

    void CoreCommand::terminate() {
        SR_CORE_MESSAGE("Terminating Syrius")
    }

    void CoreCommand::initGlad() {
        if (!m_GladInstances){
            gladLoaderLoadGL();
        }
        m_GladInstances++;
    }

    void CoreCommand::terminateGlad() {
        m_GladInstances--;
        if (!m_GladInstances){
            gladLoaderUnloadGL();
        }
    }

    void CoreCommand::initPlatformGlad(GlPlatformDesc* glDesc){
        SR_CORE_PRECONDITION(m_PlatformAPI, "Cannot initialize platform glad, no platform API was created");

        m_PlatformAPI->initPlatformGlad(glDesc);
    }

    void CoreCommand::terminatePlatformGlad() {
        SR_CORE_PRECONDITION(m_PlatformAPI, "Cannot initialize platform glad, no platform API was created");

        m_PlatformAPI->terminatePlatformGlad();
    }

    Time CoreCommand::getStartupTime() {
        return m_StartupTime;
    }

    Time CoreCommand::getElapsedTimeSinceStart() {
        return getTimeSinceEpochMilli() - m_StartupTime;
    }

    uint32 CoreCommand::getPrimaryScreenWidth() {
        return m_PlatformAPI->getPrimaryScreenWidth();
    }

    uint32 CoreCommand::getPrimaryScreenHeight() {
        return m_PlatformAPI->getPrimaryScreenHeight();
    }

    SyriusWindow *CoreCommand::createWindow(const WindowDesc &windowDesc) {
        return m_PlatformAPI->createWindow(windowDesc);
    }
}