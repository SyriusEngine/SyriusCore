#include "CoreCommand.hpp"
#include "PlatformAPIWin32Impl.hpp"

namespace Syrius{

    Time CoreCommand::m_StartupTime = 0;
    uint32 CoreCommand::m_GladInstances = 0;
    uint32 CoreCommand::m_CoreCommandInstances = 0;
    uint32 CoreCommand::m_VulkanInstances = 0;

    PlatformAPI* CoreCommand::m_PlatformAPI = nullptr;
    VulkanInstance* CoreCommand::m_VulkanInstance = nullptr;


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
        if (m_GladInstances > 0){
            SR_CORE_WARNING("Syrius is terminated but some objects still depend on glad, glad will be terminated too!");
            m_GladInstances = 0;
            gladLoaderUnloadGL();
        }

        if (m_VulkanInstances > 0){
            SR_CORE_WARNING("Syrius is terminated but some objects still depend on vulkan, vulkan will be terminated too!");

            m_VulkanInstances = 0;
            delete m_VulkanInstance;
            m_VulkanInstance = nullptr;
        }

        SR_CORE_MESSAGE("Terminating Syrius")
    }

    void CoreCommand::initGlad() {
        if (!m_GladInstances){
            gladLoaderLoadGL();

#if defined(SR_DEBUG_MODE)
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(DebugMessageHandler::pushOpenGLMessageCallback, nullptr);

#endif
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

    void CoreCommand::initVulkan() {
        SR_CORE_PRECONDITION(m_PlatformAPI, "Cannot initialize vulkan, no platform API was created");

        if (!m_VulkanInstances){
            SR_CORE_MESSAGE("initializing vulkan instance");
            m_VulkanInstance = new VulkanInstance(m_PlatformAPI);
        }
        m_VulkanInstances++;

        SR_CORE_POSTCONDITION(m_VulkanInstance, "Failed to initialize Vulkan");
    }

    void CoreCommand::terminateVulkan() {
        m_VulkanInstances--;

        if (!m_VulkanInstances){
            SR_CORE_MESSAGE("terminating vulkan instance");
            delete m_VulkanInstance;
        }
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

    std::vector<std::string> CoreCommand::getVulkanExtensionNames() {
        return m_VulkanInstance->getSupportedExtensions();
    }

}