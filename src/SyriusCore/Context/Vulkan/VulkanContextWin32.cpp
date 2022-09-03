#include "VulkanContextWin32.hpp"

#if defined(SR_PLATFORM_WIN64)

#define VK_USE_PLATFORM_WIN32_KHR

namespace Syrius{

    typedef VkFlags VkWin32SurfaceCreateFlagsKHR;

    struct VkWin32SurfaceCreateInfoKHR {
        VkStructureType                 sType;
        const void*                     pNext;
        VkWin32SurfaceCreateFlagsKHR    flags;
        HINSTANCE                       hinstance;
        HWND                            hwnd;
    };

    typedef VkResult (APIENTRY *PFN_vkCreateWin32SurfaceKHR)(VkInstance,const VkWin32SurfaceCreateInfoKHR*,const VkAllocationCallbacks*,VkSurfaceKHR*);

    VulkanContextWin32::VulkanContextWin32(HWND &hwnd)
    : VulkanContext(),
    m_Hwnd(hwnd){
        // find the win32CreateSurface function
        auto vkCreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR) vkGetInstanceProcAddr(m_Instance, "vkCreateWin32SurfaceKHR");

        if (vkCreateWin32SurfaceKHR != nullptr){
            VkWin32SurfaceCreateInfoKHR createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
            createInfo.hwnd = m_Hwnd;
            createInfo.hinstance = GetModuleHandleW(nullptr);

            SR_VULKAN_CALL(vkCreateWin32SurfaceKHR(m_Instance, &createInfo, nullptr, &m_Surface), "Failed to create Surface");

            createDevicesAndSwapChain();

        }
        else{
            SR_CORE_EXCEPTION("Failed to create a surface for Win32");
        }
    }

    VulkanContextWin32::~VulkanContextWin32() {

    }

    void VulkanContextWin32::swapBuffers() {

    }

    void VulkanContextWin32::setVerticalSynchronisation(bool enable) {

    }

    void VulkanContextWin32::createImGuiContext() {

    }

    void VulkanContextWin32::destroyImGuiContext() {

    }

    void VulkanContextWin32::onImGuiBegin() {

    }

    void VulkanContextWin32::onImGuiEnd() {

    }

    FramebufferSize VulkanContextWin32::getFramebufferSize() {
        RECT area;
        GetClientRect(m_Hwnd, &area);
        FramebufferSize size;
        size.m_Width = area.right;
        size.m_Height = area.bottom;
        return size;
    }

}

#endif