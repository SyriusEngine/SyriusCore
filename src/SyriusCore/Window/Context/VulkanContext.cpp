#include "VulkanContext.hpp"

namespace Syrius{

    VulkanContext::VulkanContext()
    : Context(SR_API_VULKAN),
    m_Surface(nullptr),
    m_Instance(nullptr),
    m_Device(nullptr){
        CoreCommand::initVulkan();

        m_Instance = CoreCommand::getVulkanInstance()->getInstance();
        m_Device = CoreCommand::getVulkanLogicalDevice()->getDevice();

    }

    VulkanContext::~VulkanContext() {
        if (m_Surface != nullptr){

        }
    }

    VkSurfaceKHR &VulkanContext::getSurface() {
        return m_Surface;
    }
}

