//
// Created by Codex on 23/07/2026.
//

#pragma once

#include <vulkan/vulkan.h>

#include <cstdint>
#include <optional>
#include <vector>

namespace Sunset
{
    struct VulkanQueueFamilyIndices
    {
        std::optional<std::uint32_t> graphicsFamily;
        std::optional<std::uint32_t> presentFamily;

        [[nodiscard]] bool IsComplete() const;
    };

    struct VulkanSwapchainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities{};
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class VulkanDevice
    {
    public:
        VulkanDevice(VkInstance instance, VkSurfaceKHR surface);
        ~VulkanDevice();

        VulkanDevice(const VulkanDevice&) = delete;
        VulkanDevice& operator=(const VulkanDevice&) = delete;
        VulkanDevice(VulkanDevice&&) = delete;
        VulkanDevice& operator=(VulkanDevice&&) = delete;

        [[nodiscard]] VkPhysicalDevice GetPhysicalDevice() const;
        [[nodiscard]] VkDevice GetDevice() const;
        [[nodiscard]] VkQueue GetGraphicsQueue() const;
        [[nodiscard]] VkQueue GetPresentQueue() const;
        [[nodiscard]] const VulkanQueueFamilyIndices& GetQueueFamilies() const;

        [[nodiscard]] VulkanSwapchainSupportDetails QuerySwapchainSupport() const;

    private:
        static const std::vector<const char*>& RequiredDeviceExtensions();

        [[nodiscard]] bool IsDeviceSuitable(VkPhysicalDevice physicalDevice) const;
        [[nodiscard]] bool CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice) const;
        [[nodiscard]] VulkanQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice physicalDevice) const;
        [[nodiscard]] VulkanSwapchainSupportDetails QuerySwapchainSupport(VkPhysicalDevice physicalDevice) const;

        void PickPhysicalDevice();
        void CreateLogicalDevice();

        VkInstance m_Instance = VK_NULL_HANDLE;
        VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
        VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
        VkDevice m_Device = VK_NULL_HANDLE;
        VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
        VkQueue m_PresentQueue = VK_NULL_HANDLE;
        VulkanQueueFamilyIndices m_QueueFamilies;
    };
}
