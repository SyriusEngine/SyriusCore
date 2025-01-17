#pragma once

#include "../GlContext.hpp"
#include "../../../../../include/SyriusCore/Dependencies/imgui/imgui.h"
#include "../../../Dependencies/imgui/imgui_impl_opengl3.h"

#if !defined(SR_PLATFORM_WIN64)

namespace Syrius {

    class Glfw3glContext: public GlContext {
    public:
        Glfw3glContext(GLFWwindow* window, const ContextDesc& desc);

        ~Glfw3glContext() override;

        static void setGlfw3glContextHints(const ContextDesc& desc);

        void makeCurrent() override;

        void swapBuffers() override;

        void setVerticalSynchronisation(bool enable) override;

        FramebufferSize getFramebufferSize() override;

    protected:

        void createImGuiContext() override;

        void destroyImGuiContext() override;

        void onImGuiBegin() override;

        void onImGuiEnd() override;

    private:
        GLFWwindow* m_Window = nullptr;
        ImGuiContext *m_ImGuiContext = nullptr;
    };

}

#endif