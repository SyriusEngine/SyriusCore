#include "VulkanInstance.hpp"

namespace Syrius{

    VulkanInstance::VulkanInstance(const PlatformAPI *platformAPI)
    : m_PlatformAPI(platformAPI),
      m_Instance(nullptr),
      m_DebugMessenger(nullptr){
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Syrius Core Lib";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "SyriusAPI";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        const VulkanPlatformDesc& vpDesc = m_PlatformAPI->getVulkanPlatformDesc();

        const char** extensions = new const char*[vpDesc.getExtensionCount()];
        for (uint32 i = 0; i < vpDesc.getExtensionCount(); i++){
            extensions[i] = vpDesc.getExtensionNames()[i].c_str();
        }

        const char** layers = new const char*[vpDesc.getLayerCount()];
        for (uint32 i = 0; i < vpDesc.getLayerCount(); i++){
            layers[i] = vpDesc.getLayerNames()[i].c_str();
        }

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = vpDesc.getExtensionCount();
        createInfo.ppEnabledExtensionNames = extensions;
        createInfo.enabledLayerCount = vpDesc.getLayerCount();
        createInfo.ppEnabledLayerNames = layers;

        SR_VULKAN_CALL(vkCreateInstance(&createInfo, nullptr, &m_Instance), "Failed to create the vulkan instance");

#if defined(SR_DEBUG_MODE)
        setupDebugMessenger();
#endif
        delete[] layers;
        delete[] extensions;

    }

    VulkanInstance::~VulkanInstance() {
        if (m_DebugMessenger){
            destroyDebugUtilsMessenger(m_DebugMessenger, nullptr);
        }

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

    std::vector<std::string> VulkanInstance::getSupportedLayers() const {
        uint32 layerCount = 0;
        SR_VULKAN_CALL(vkEnumerateInstanceLayerProperties(&layerCount, nullptr), "Failed to get the number of validation layers");

        std::vector<VkLayerProperties> layers(layerCount);
        SR_VULKAN_CALL(vkEnumerateInstanceLayerProperties(&layerCount, layers.data()), "Failed to get the layer information");

        std::vector<std::string> layerNames;
        for (const auto& layer : layers){
            layerNames.emplace_back(layer.layerName);
        }

        return layerNames;
    }

    void VulkanInstance::setupDebugMessenger() {
        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity =    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType =    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pUserData = nullptr;
        createInfo.pfnUserCallback = DebugMessageHandler::vulkanDebugCallback;

        SR_VULKAN_CALL(createDebugUtilsMessenger(&createInfo, nullptr, &m_DebugMessenger), "Failed to create the vulkan debug messenger")

    }

    VkResult VulkanInstance::createDebugUtilsMessenger(const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                                       const VkAllocationCallbacks *pAllocator,
                                                       VkDebugUtilsMessengerEXT *pDebugMessenger) {
        auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_Instance, "vkCreateDebugUtilsMessengerEXT");
        if (vkCreateDebugUtilsMessengerEXT != nullptr){
            return vkCreateDebugUtilsMessengerEXT(m_Instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else{
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void VulkanInstance::destroyDebugUtilsMessenger(VkDebugUtilsMessengerEXT debugMessenger,
                                                        const VkAllocationCallbacks *pAllocator) {
        auto vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_Instance, "vkDestroyDebugUtilsMessengerEXT");
        if (vkDestroyDebugUtilsMessengerEXT != nullptr){
            vkDestroyDebugUtilsMessengerEXT(m_Instance, debugMessenger, pAllocator);
        }
    }

    VkInstance& VulkanInstance::getInstance() {
        return m_Instance;
    }

}