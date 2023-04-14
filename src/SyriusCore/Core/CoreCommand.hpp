#pragma once

#include "../../../../include/SyriusCore/Core/PlatformAPI.hpp"
#include "Vulkan/VulkanInterface.hpp"
#include "../../../include/SyriusCore/Utils/Resource.hpp"

namespace Syrius{

    class CoreCommand{
    public:
        CoreCommand();

        ~CoreCommand();

        Time getStartupTime();

        Time getElapsedTimeSinceStart();

        uint32 getPrimaryScreenWidth();

        uint32 getPrimaryScreenHeight();

        Resource<SyriusWindow> createWindow(const WindowDesc& windowDesc);

    private:
        static CoreCommand* m_Instance;

        Time m_StartupTime;
        uint32 m_GladInstances;

        Resource<PlatformAPI> m_PlatformAPI;
    };

}

