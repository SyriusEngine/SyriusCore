#include "VulkanContext.hpp"

namespace Syrius{

    bool QueueFamilyIndices::isComplete() {
        return m_GraphicsFamily.has_value() && m_PresentFamily.has_value();
    }

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
        if (m_Device != nullptr){
            vkDestroyDevice(m_Device, nullptr);
        }
        if (m_Surface != nullptr){
            vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
        }

        CoreCommand::terminateVulkan();
    }

    VkSurfaceKHR &VulkanContext::getSurface() {
        return m_Surface;
    }

    void VulkanContext::selectPhysicalDevice() {
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

            m_PhysicalDevice = candidates.begin()->second;
        }
        else{
            SR_CORE_EXCEPTION("No GPUs found who support vulkan")
        }

        SR_CORE_POSTCONDITION(m_Device != VK_NULL_HANDLE, "Failed to find a suitable physical device");
    }

    uint32 VulkanContext::ratePhysicalDevice(VkPhysicalDevice device) {
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


        auto queueFamilyIndices = findQueueFamily(device);
        score += queueFamilyIndices.isComplete();

        return score;
    }

    QueueFamilyIndices VulkanContext::findQueueFamily(VkPhysicalDevice device) {
        QueueFamilyIndices indices;

        uint32 queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyProperties.data());

        int32 i = 0;
        for (const auto& queueFamily: queueFamilyProperties){
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT){
                indices.m_GraphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_Surface, &presentSupport);
            if (presentSupport){
                indices.m_PresentFamily = i;
            }

            if (indices.isComplete()){
                break;
            }
            i++;
        }

        return indices;
    }

    void VulkanContext::createLogicalDevice() {
        QueueFamilyIndices indices = findQueueFamily(m_PhysicalDevice);
        float queuePriority = 1.0f;
        auto validationLayers = CoreCommand::getVulkanInstance()->getSupportedLayers();

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32> uniqueQueueFamilies = {indices.m_GraphicsFamily.value(), indices.m_PresentFamily.value()};
        for (uint32 queueFamily : uniqueQueueFamilies){
            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.queueCreateInfoCount = queueCreateInfos.size();
        createInfo.pEnabledFeatures = &deviceFeatures;

        SR_VULKAN_CALL(vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device), "Failed to create logical device");

        vkGetDeviceQueue(m_Device, indices.m_GraphicsFamily.value(), 0, &m_GraphicsQueue);
        vkGetDeviceQueue(m_Device, indices.m_PresentFamily.value(), 0, &m_PresentQueue);
    }

}

