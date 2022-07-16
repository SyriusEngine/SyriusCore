#include "VulkanInstance.hpp"

namespace Syrius{

    VulkanInstance::VulkanInstance(const PlatformAPI *platformAPI)
    : m_PlatformAPI(platformAPI),
      m_Instance(nullptr){
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "SyriusAPI Runtime";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "SyriusAPI";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        const VulkanPlatformDesc& vpDesc = m_PlatformAPI->getVulkanPlatformDesc();
        const char** extensions = new const char*[vpDesc.m_ExtensionCount];
        for (uint32 i = 0; i < vpDesc.m_ExtensionCount; i++){
            extensions[i] = vpDesc.m_Extensions[i].c_str();
        }

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = vpDesc.m_ExtensionCount;
        createInfo.ppEnabledExtensionNames = extensions;

        SR_VULKAN_CALL(vkCreateInstance(&createInfo, nullptr, &m_Instance), "Failed to create the vulkan instance");

    }

    VulkanInstance::~VulkanInstance() {
        if (m_Instance){
            vkDestroyInstance(m_Instance, nullptr);
        }

    }

    std::vector<std::string> VulkanInstance::getSupportedExtensions() const {
        uint32 extensionCount = 0;
        SR_VULKAN_CALL(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr), "Failed to get the number of extensions");

        std::vector<VkExtensionProperties> extensions(extensionCount);
        SR_VULKAN_CALL(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data()), "Failed to get extension information");

        std::vector<std::string> extensionNames(extensionCount);
        for (const auto& ext : extensions){
            extensionNames.emplace_back(ext.extensionName);
        }

        return extensionNames;
    }

    const VkInstance &VulkanInstance::getVkInstance() const {
        return m_Instance;
    }
}