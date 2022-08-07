#pragma once

#include "VulkanContext.hpp"

namespace Syrius{

    class VulkanContextWin32: public VulkanContext{
    public:

        explicit VulkanContextWin32(HWND& hwnd);

        ~VulkanContextWin32() override;

        void swapBuffers() override;

        void setVerticalSynchronisation(bool enable) override;

        void createImGuiContext() override;

        void destroyImGuiContext() override;

        void onImGuiBegin() override;

        void onImGuiEnd() override;

    private:
        HWND& m_Hwnd;
    };

}

