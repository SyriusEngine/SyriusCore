#include "VulkanPhysicalDevice.hpp"

namespace Syrius{

    VulkanPhysicalDevice::VulkanPhysicalDevice(VkInstance instance, VkSurfaceKHR surface)
    : m_Device(nullptr),
    m_Instance(instance),
    m_Surface(surface),
    m_Extensions({
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    }){
        uint32 deviceCount = 0;
        SR_VULKAN_CALL(vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr), "Failed to get the number of available vulkan supported GPUs");

        if (deviceCount > 0){
            std::vector<VkPhysicalDevice> devices(deviceCount);
            SR_VULKAN_CALL(vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data()), "Failed to retrieve al available physical devices");

            std::multimap<uint32, VkPhysicalDevice> candidates;

            for (const auto& device : devices){
                uint32 score = ratePhysicalDevice(device);
                candidates.insert({score, device});
            }

            if (candidates.begin()->first > 0){
                m_Device = candidates.begin()->second;
            }
            else{
                SR_CORE_EXCEPTION("Failed to find a suitable physical device");
            }
        }
        else{
            SR_CORE_EXCEPTION("No GPUs found who support vulkan")
        }

        SR_CORE_POSTCONDITION(m_Device != VK_NULL_HANDLE, "Failed to find a suitable physical device");
    }

    VulkanPhysicalDevice::~VulkanPhysicalDevice() {

    }

    VkPhysicalDevice VulkanPhysicalDevice::getPhysicalDevice() const {
        return m_Device;
    }

    uint32 VulkanPhysicalDevice::ratePhysicalDevice(VkPhysicalDevice device) {
        uint32 score = 0;
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;

        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        switch(deviceProperties.deviceType){
            case VK_PHYSICAL_DEVICE_TYPE_OTHER:             score += 0; break;
            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:    score += 400; break;
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:      score += 1000; break;
            case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:       score += 600; break;
            case VK_PHYSICAL_DEVICE_TYPE_CPU:               score += 50; break;
            case VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM:          score += 0; break;
        }

        // TODO: improve Physical device selection process


        auto queueFamilyIndices = findQueueFamily(device, m_Surface);
        score += queueFamilyIndices.isComplete();

        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, m_Surface);
        bool swapChainAdequate = !swapChainSupport.m_Formats.empty() && !swapChainSupport.m_PresentModes.empty();
        if (!checkDeviceExtensionSupport(device) or !swapChainAdequate){
            score *= -1;
        }

        return score;
    }

    VkPhysicalDeviceProperties VulkanPhysicalDevice::getDeviceProperties() const {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(m_Device, &properties);
        return properties;
    }

    VkPhysicalDeviceLimits VulkanPhysicalDevice::getDeviceLimits() const {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(m_Device, &properties);
        return properties.limits;
    }

    const std::vector<std::string> &VulkanPhysicalDevice::getExtensionsNames() const {
        return m_Extensions;
    }

    bool VulkanPhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device) {
        uint32 extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(m_Extensions.begin(), m_Extensions.end());

        for (const auto& extension : availableExtensions){
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    QueueFamilyIndices VulkanPhysicalDevice::getQueueFamilyIndices() const {
        return findQueueFamily(m_Device, m_Surface);
    }
}

