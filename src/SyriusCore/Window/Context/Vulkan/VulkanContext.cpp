#include "VulkanContext.hpp"

namespace Syrius{

    VulkanContext::VulkanContext()
    : Context(SR_API_VULKAN),
    m_Surface(nullptr),
    m_Instance(nullptr),
    m_PhysicalDevice(nullptr),
    m_Device(nullptr){
        CoreCommand::initVulkan();

        m_Instance = CoreCommand::getVulkanInstance()->getInstance();

    }

    VulkanContext::~VulkanContext() {
        if (m_Surface != nullptr){
            vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
        }

        delete m_Device;
        delete m_PhysicalDevice;

        CoreCommand::terminateVulkan();
    }

    void VulkanContext::createDevicesAndSwapChain() {
        /// ALWAYS CALL THIS FUNCTION IN THE CONSTRUCTOR OF THE CHILD CLASS

        m_PhysicalDevice = new VulkanPhysicalDevice(m_Instance, m_Surface);
        m_Device = new VulkanLogicalDevice(m_PhysicalDevice, m_Surface);
    }

    std::string VulkanContext::getAPIVersion() {
        return std::to_string(m_PhysicalDevice->getDeviceProperties().apiVersion);
    }

    std::string VulkanContext::getDeviceName() {
        return m_PhysicalDevice->getDeviceProperties().deviceName;
    }

    std::string VulkanContext::getShadingLanguageVersion() {
        return {};
    }

    int32 VulkanContext::getMaxFramebufferWidth() {
        return m_PhysicalDevice->getDeviceProperties().limits.maxFramebufferWidth;
    }

    int32 VulkanContext::getMaxFramebufferHeight() {
        return m_PhysicalDevice->getDeviceProperties().limits.maxFramebufferHeight;
    }

    int32 VulkanContext::getMaxFramebufferTextureAttachments() {
        return m_PhysicalDevice->getDeviceProperties().limits.maxColorAttachments;
    }

    int32 VulkanContext::getMaxTextureSlots() {
        return m_PhysicalDevice->getDeviceProperties().limits.maxPerStageDescriptorSampledImages;
    }

    int32 VulkanContext::getMaxTexture2DSize() {
        return m_PhysicalDevice->getDeviceProperties().limits.maxImageDimension2D;
    }

    int32 VulkanContext::getMaxConstantBufferSize() {
        return m_PhysicalDevice->getDeviceProperties().limits.maxUniformBufferRange;
    }

    int32 VulkanContext::getMaxDepthBufferBits() {
        return 0;
    }

}

