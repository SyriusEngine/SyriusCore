#pragma once

#include "../GlContext.hpp"
#include "../../../../../include/SyriusCore/Dependencies/imgui/imgui.h"
#include "../../../Dependencies/imgui/imgui_impl_opengl3.h"

#if defined(SR_CORE_PLATFORM_LINUX)

// #include "../../../Dependencies/imgui/imgui_impl_x11.h"
#include "../../../Dependencies/glad/glad/glx.h"

namespace Syrius{

    class GlxContext: public GlContext{
    public:
        GlxContext(Display* display, GLXFBConfig fbConfig, Window& window, const ContextDesc& desc);

        ~GlxContext() override;

        void makeCurrent() override;

        void swapBuffers() override;

        void setVerticalSynchronisation(bool enable) override;

        FramebufferSize getFramebufferSize() override;

        static void initGLX(Display* display);

        static void terminateGLX();

    protected:

        void createImGuiContext() override;

        void destroyImGuiContext() override;

        void onImGuiBegin() override;

        void onImGuiEnd() override;

    private:
        static uint32 m_ContextCount;
        static int32 m_GlxVersion;

        Display* m_Display;
        Window& m_Window;

        GLXContext m_Context;

    };

}

#endif