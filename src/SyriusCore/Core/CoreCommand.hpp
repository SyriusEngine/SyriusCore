#pragma once

#include "../../../../include/SyriusCore/Core/PlatformAPI.hpp"
#include "VulkanInstance.hpp"

namespace Syrius{

    class CoreCommand{
    public:
        CoreCommand() = delete;

        ~CoreCommand() = delete;

        static void init();

        static void terminate();

        static void initGlad();

        static void terminateGlad();

        static void initPlatformGlad(GlPlatformDesc* glDesc);

        static void terminatePlatformGlad();

        static void initVulkan();

        static void terminateVulkan();

        static Time getStartupTime();

        static Time getElapsedTimeSinceStart();

        static uint32 getPrimaryScreenWidth();

        static uint32 getPrimaryScreenHeight();

        static SyriusWindow* createWindow(const WindowDesc& windowDesc);

        static std::vector<std::string> getVulkanExtensionNames();

    private:
        static Time m_StartupTime;
        static uint32 m_CoreCommandInstances;
        static uint32 m_GladInstances;
        static uint32 m_VulkanInstances;

        static PlatformAPI* m_PlatformAPI;
        static VulkanInstance* m_VulkanInstance;
    };

}

