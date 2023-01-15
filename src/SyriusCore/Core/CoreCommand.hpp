#pragma once

#include "../../../../include/SyriusCore/Core/PlatformAPI.hpp"
#include "Vulkan/VulkanInterface.hpp"
#include "../../../include/SyriusCore/Utils/Resource.hpp"

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

        static Time getStartupTime();

        static Time getElapsedTimeSinceStart();

        static uint32 getPrimaryScreenWidth();

        static uint32 getPrimaryScreenHeight();

        static SyriusWindow* createWindow(const WindowDesc& windowDesc);

        static void destroyWindow(SyriusWindow* window);

        static ResourceView<Image> createImage(const std::string& fileName, bool flipOnLoad = true);

        static ResourceView<Image> createImage(const ubyte* pixelData, int32 width, int32 height, int32 channelCount);

    private:
        static Time m_StartupTime;
        static uint32 m_CoreCommandInstances;
        static uint32 m_GladInstances;
        static uint32 m_VulkanInterfaces;

        static PlatformAPI* m_PlatformAPI;

        static std::vector<SyriusWindow*> m_WindowInstances;

        static std::vector<Resource<Image>> m_Images;
    };

}

