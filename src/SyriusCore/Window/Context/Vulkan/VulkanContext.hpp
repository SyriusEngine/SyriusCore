#pragma once

#include <optional>
#include <set>

#include "../../../../../include/SyriusCore/Window/Context/Context.hpp"
#include "../../../Core/CoreCommand.hpp"
#include "VulkanUtils.hpp"
#include "VulkanLogicalDevice.hpp"
#include "VulkanPhysicalDevice.hpp"

namespace Syrius{

    class VulkanContext: public Context{
    public:

        VulkanContext();

        ~VulkanContext() override;

        std::string getAPIVersion() override;

        std::string getDeviceName() override;

        std::string getShadingLanguageVersion() override;

        int32 getMaxFramebufferWidth() override;

        int32 getMaxFramebufferHeight() override;

        int32 getMaxFramebufferTextureAttachments() override;

        int32 getMaxTextureSlots() override;

        int32 getMaxTexture2DSize() override;

        int32 getMaxConstantBufferSize() override;

        int32 getMaxDepthBufferBits() override;

    protected:

        void createDevicesAndSwapChain();

    protected:
        VkInstance m_Instance;
        VkSurfaceKHR m_Surface;

        VulkanPhysicalDevice* m_PhysicalDevice;
        VulkanLogicalDevice* m_Device;
    };

}

