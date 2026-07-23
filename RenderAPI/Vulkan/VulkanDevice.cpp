//
// Created by Codex on 23/07/2026.
//

#include "VulkanDevice.h"

#include "VulkanDebug.h"

#include <set>
#include <string>
#include <stdexcept>

namespace Sunset
{
    bool VulkanQueueFamilyIndices::IsComplete() const
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }

    VulkanDevice::VulkanDevice(const VkInstance instance, const VkSurfaceKHR surface)
        : m_Instance(instance)
        , m_Surface(surface)
    {
        PickPhysicalDevice();
        CreateLogicalDevice();
    }

    VulkanDevice::~VulkanDevice()
    {
        if (m_Device != VK_NULL_HANDLE)
            vkDestroyDevice(m_Device, nullptr);
    }

    VkPhysicalDevice VulkanDevice::GetPhysicalDevice() const
    {
        return m_PhysicalDevice;
    }

    VkDevice VulkanDevice::GetDevice() const
    {
        return m_Device;
    }

    VkQueue VulkanDevice::GetGraphicsQueue() const
    {
        return m_GraphicsQueue;
    }

    VkQueue VulkanDevice::GetPresentQueue() const
    {
        return m_PresentQueue;
    }

    const VulkanQueueFamilyIndices& VulkanDevice::GetQueueFamilies() const
    {
        return m_QueueFamilies;
    }

    VulkanSwapchainSupportDetails VulkanDevice::QuerySwapchainSupport() const
    {
        return QuerySwapchainSupport(m_PhysicalDevice);
    }

    const std::vector<const char*>& VulkanDevice::RequiredDeviceExtensions()
    {
        static const std::vector extensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        return extensions;
    }

    bool VulkanDevice::IsDeviceSuitable(const VkPhysicalDevice physicalDevice) const
    {
        const VulkanQueueFamilyIndices indices = FindQueueFamilies(physicalDevice);
        const bool extensionsSupported = CheckDeviceExtensionSupport(physicalDevice);

        bool swapchainAdequate = false;
        if (extensionsSupported)
        {
            const VulkanSwapchainSupportDetails swapchainSupport = QuerySwapchainSupport(physicalDevice);
            swapchainAdequate = !swapchainSupport.formats.empty() && !swapchainSupport.presentModes.empty();
        }

        return indices.IsComplete() && extensionsSupported && swapchainAdequate;
    }

    bool VulkanDevice::CheckDeviceExtensionSupport(const VkPhysicalDevice physicalDevice) const
    {
        std::uint32_t extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(RequiredDeviceExtensions().begin(), RequiredDeviceExtensions().end());
        for (const VkExtensionProperties& extension : availableExtensions)
            requiredExtensions.erase(extension.extensionName);

        return requiredExtensions.empty();
    }

    VulkanQueueFamilyIndices VulkanDevice::FindQueueFamilies(const VkPhysicalDevice physicalDevice) const
    {
        VulkanQueueFamilyIndices indices;

        std::uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

        for (std::uint32_t index = 0; index < queueFamilyCount; ++index)
        {
            if ((queueFamilies[index].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
                indices.graphicsFamily = index;

            VkBool32 presentSupport = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, index, m_Surface, &presentSupport);
            if (presentSupport == VK_TRUE)
                indices.presentFamily = index;

            if (indices.IsComplete())
                break;
        }

        return indices;
    }

    VulkanSwapchainSupportDetails VulkanDevice::QuerySwapchainSupport(const VkPhysicalDevice physicalDevice) const
    {
        VulkanSwapchainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, m_Surface, &details.capabilities);

        std::uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_Surface, &formatCount, nullptr);
        if (formatCount != 0)
        {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_Surface, &formatCount, details.formats.data());
        }

        std::uint32_t presentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_Surface, &presentModeCount, nullptr);
        if (presentModeCount != 0)
        {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_Surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    void VulkanDevice::PickPhysicalDevice()
    {
        std::uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);

        if (deviceCount == 0)
            throw std::runtime_error("Failed to find GPUs with Vulkan support");

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());

        for (const VkPhysicalDevice device : devices)
        {
            if (IsDeviceSuitable(device))
            {
                m_PhysicalDevice = device;
                m_QueueFamilies = FindQueueFamilies(device);
                return;
            }
        }

        throw std::runtime_error("Failed to find a suitable Vulkan GPU");
    }

    void VulkanDevice::CreateLogicalDevice()
    {
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        const std::set uniqueQueueFamilies = {
            m_QueueFamilies.graphicsFamily.value(),
            m_QueueFamilies.presentFamily.value()
        };

        constexpr float queuePriority = 1.0f;
        for (const std::uint32_t queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.emplace_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};

        const std::vector<const char*>& deviceExtensions = RequiredDeviceExtensions();

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<std::uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<std::uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        if (VulkanDebug::ValidationLayersEnabled())
        {
            const std::vector<const char*>& validationLayers = VulkanDebug::ValidationLayers();
            createInfo.enabledLayerCount = static_cast<std::uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }

        if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device) != VK_SUCCESS)
            throw std::runtime_error("Failed to create Vulkan logical device");

        vkGetDeviceQueue(m_Device, m_QueueFamilies.graphicsFamily.value(), 0, &m_GraphicsQueue);
        vkGetDeviceQueue(m_Device, m_QueueFamilies.presentFamily.value(), 0, &m_PresentQueue);
    }
}
