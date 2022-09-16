#pragma once

#include "VulkanContext.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    class VulkanContextWin32: public VulkanContext{
    public:

        explicit VulkanContextWin32(HWND& hwnd, const ContextDesc& desc);

        ~VulkanContextWin32() override;

        void swapBuffers() override;

        void setVerticalSynchronisation(bool enable) override;

        void createImGuiContext() override;

        void destroyImGuiContext() override;

        void onImGuiBegin() override;

        void onImGuiEnd() override;

        FramebufferSize getFramebufferSize() override;

    private:
        HWND& m_Hwnd;
    };

}

#endif

