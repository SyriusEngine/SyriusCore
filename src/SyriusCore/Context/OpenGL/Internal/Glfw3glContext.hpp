#pragma once

#include "../GlContext.hpp"
#include "../../../../../include/SyriusCore/Dependencies/imgui/imgui.h"

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

        void initImGui(const ImGuiDesc& desc) override;

        void terminateImGui() override;

        void onImGuiBegin() override;

        void onImGuiEnd() override;

    private:
        GLFWwindow* m_Window = nullptr;
    };

}

#endif