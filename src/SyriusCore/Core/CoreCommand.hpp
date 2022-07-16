#pragma once

#include "../../../../include/SyriusCore/Core/PlatformAPI.hpp"

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

    private:
        static PlatformAPI* m_PlatformAPI;
        static Time m_StartupTime;
        static uint32 m_CoreCommandInstances;
        static uint32 m_GladInstances;
    };

}

