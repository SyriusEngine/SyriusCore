#include "VulkanPhysicalDevice.hpp"

namespace Syrius{

    VulkanPhysicalDevice::VulkanPhysicalDevice(VkInstance& instance)
    : m_Instance(instance),
    m_Device(VK_NULL_HANDLE){
        uint32 deviceCount = 0;
        SR_VULKAN_CALL(vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr), "Failed to get the number of available vulkan supported GPUs");

        if (deviceCount > 0){
            std::vector<VkPhysicalDevice> devices(deviceCount);
            SR_VULKAN_CALL(vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data()), "Failed to retrieve al available physical devices");

            std::multimap<uint32, VkPhysicalDevice> candidates;

            for (const auto& device : devices){
                uint32 score = rateDevice(device);
                candidates.insert({score, device});
            }

            m_Device = candidates.begin()->second;
        }
        else{
            SR_CORE_EXCEPTION("No GPUs found who support vulkan")
        }

        SR_CORE_POSTCONDITION(m_Device != VK_NULL_HANDLE, "Failed to find a suitable physical device");
    }

    VulkanPhysicalDevice::~VulkanPhysicalDevice() {

    }

    VkPhysicalDevice &VulkanPhysicalDevice::getDevice() {
        return m_Device;
    }

    uint32 VulkanPhysicalDevice::rateDevice(VkPhysicalDevice device) {
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


}

