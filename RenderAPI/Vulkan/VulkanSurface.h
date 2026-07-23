//
// Created by Codex on 23/07/2026.
//

#pragma once

#include <vulkan/vulkan.h>

namespace Sunset
{
    class VulkanSurface
    {
    public:
        VulkanSurface(VkInstance instance, void* nativeWindow);
        ~VulkanSurface();

        VulkanSurface(const VulkanSurface&) = delete;
        VulkanSurface& operator=(const VulkanSurface&) = delete;
        VulkanSurface(VulkanSurface&&) = delete;
        VulkanSurface& operator=(VulkanSurface&&) = delete;

        [[nodiscard]] VkSurfaceKHR Get() const;

    private:
        VkInstance m_Instance = VK_NULL_HANDLE;
        VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
    };
}
