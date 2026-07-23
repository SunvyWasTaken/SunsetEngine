//
// Created by Codex on 23/07/2026.
//

#pragma once

#include <vulkan/vulkan.h>

#include <vector>

namespace Sunset
{
    namespace VulkanDebug
    {
        [[nodiscard]] bool ValidationLayersRequested();
        [[nodiscard]] bool ValidationLayersEnabled();
        [[nodiscard]] const std::vector<const char*>& ValidationLayers();
        [[nodiscard]] bool CheckValidationLayerSupport();

        void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        void CreateDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT& debugMessenger);
        void DestroyDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger);
    }
}
