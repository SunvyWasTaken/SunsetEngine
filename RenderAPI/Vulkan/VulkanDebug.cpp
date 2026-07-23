//
// Created by Codex on 23/07/2026.
//

#include "VulkanDebug.h"

#include <cstring>
#include <iostream>
#include <stdexcept>

namespace
{
    VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT,
        VkDebugUtilsMessageTypeFlagsEXT,
        const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
        void*)
    {
        std::cerr << "[Vulkan] " << (callbackData != nullptr ? callbackData->pMessage : "Validation message") << '\n';
        return VK_FALSE;
    }

    VkResult CreateDebugUtilsMessengerEXT(
        const VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* createInfo,
        const VkAllocationCallbacks* allocator,
        VkDebugUtilsMessengerEXT* debugMessenger)
    {
        const auto function = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));

        if (function == nullptr)
            return VK_ERROR_EXTENSION_NOT_PRESENT;

        return function(instance, createInfo, allocator, debugMessenger);
    }

    void DestroyDebugUtilsMessengerEXT(
        const VkInstance instance,
        const VkDebugUtilsMessengerEXT debugMessenger,
        const VkAllocationCallbacks* allocator)
    {
        const auto function = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));

        if (function != nullptr)
            function(instance, debugMessenger, allocator);
    }
}

namespace Sunset::VulkanDebug
{
    bool ValidationLayersRequested()
    {
    #ifndef NDEBUG
        return true;
    #else
        return false;
    #endif
    }

    bool ValidationLayersEnabled()
    {
        return ValidationLayersRequested() && CheckValidationLayerSupport();
    }

    const std::vector<const char*>& ValidationLayers()
    {
        static const std::vector layers = {
            "VK_LAYER_KHRONOS_validation"
        };

        return layers;
    }

    bool CheckValidationLayerSupport()
    {
        std::uint32_t layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : ValidationLayers())
        {
            bool found = false;
            for (const VkLayerProperties& layerProperties : availableLayers)
            {
                if (std::strcmp(layerName, layerProperties.layerName) == 0)
                {
                    found = true;
                    break;
                }
            }

            if (!found)
                return false;
        }

        return true;
    }

    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
    {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = DebugCallback;
    }

    void CreateDebugMessenger(const VkInstance instance, VkDebugUtilsMessengerEXT& debugMessenger)
    {
        if (!ValidationLayersEnabled())
            return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        PopulateDebugMessengerCreateInfo(createInfo);

        if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
            throw std::runtime_error("Failed to create Vulkan debug messenger");
    }

    void DestroyDebugMessenger(const VkInstance instance, const VkDebugUtilsMessengerEXT debugMessenger)
    {
        if (debugMessenger != VK_NULL_HANDLE)
            DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }
}
