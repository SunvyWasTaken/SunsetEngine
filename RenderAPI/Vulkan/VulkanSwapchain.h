//
// Created by Codex on 23/07/2026.
//

#pragma once

#include "VulkanDevice.h"

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

#include <vector>

namespace Sunset
{
    class VulkanSwapchain
    {
    public:
        VulkanSwapchain(VulkanDevice& device, VkSurfaceKHR surface, const glm::ivec2& requestedExtent);
        ~VulkanSwapchain();

        VulkanSwapchain(const VulkanSwapchain&) = delete;
        VulkanSwapchain& operator=(const VulkanSwapchain&) = delete;
        VulkanSwapchain(VulkanSwapchain&&) = delete;
        VulkanSwapchain& operator=(VulkanSwapchain&&) = delete;

        [[nodiscard]] VkSwapchainKHR Get() const;
        [[nodiscard]] VkRenderPass GetRenderPass() const;
        [[nodiscard]] VkFramebuffer GetFramebuffer(std::uint32_t imageIndex) const;
        [[nodiscard]] VkExtent2D GetExtent() const;
        [[nodiscard]] std::uint32_t GetImageCount() const;

    private:
        [[nodiscard]] static VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        [[nodiscard]] static VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        [[nodiscard]] static VkExtent2D ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities, const glm::ivec2& requestedExtent);

        void CreateSwapchain(const glm::ivec2& requestedExtent);
        void CreateImageViews();
        void CreateRenderPass();
        void CreateFramebuffers();
        void Destroy();

        VulkanDevice& m_Device;
        VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
        VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;
        VkFormat m_ImageFormat = VK_FORMAT_UNDEFINED;
        VkExtent2D m_Extent{};
        std::vector<VkImage> m_Images;
        std::vector<VkImageView> m_ImageViews;
        VkRenderPass m_RenderPass = VK_NULL_HANDLE;
        std::vector<VkFramebuffer> m_Framebuffers;
    };
}
