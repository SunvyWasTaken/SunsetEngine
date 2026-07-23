//
// Created by Codex on 23/07/2026.
//

#pragma once

#include <vulkan/vulkan.h>

#include <string_view>
#include <vector>

namespace Sunset
{
    class VulkanInstance
    {
    public:
        explicit VulkanInstance(std::string_view applicationName);
        ~VulkanInstance();

        VulkanInstance(const VulkanInstance&) = delete;
        VulkanInstance& operator=(const VulkanInstance&) = delete;
        VulkanInstance(VulkanInstance&&) = delete;
        VulkanInstance& operator=(VulkanInstance&&) = delete;

        [[nodiscard]] VkInstance Get() const;

    private:
        static std::vector<const char*> RequiredExtensions();

        VkInstance m_Instance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT m_DebugMessenger = VK_NULL_HANDLE;
    };
}
