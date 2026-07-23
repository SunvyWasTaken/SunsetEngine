//
// Created by Codex on 23/07/2026.
//

#include "VulkanInstance.h"

#include "VulkanDebug.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
    constexpr const char* SurfaceExtension = "VK_KHR_surface";
    constexpr const char* XcbSurfaceExtension = "VK_KHR_xcb_surface";
    constexpr const char* XlibSurfaceExtension = "VK_KHR_xlib_surface";
    constexpr const char* WaylandSurfaceExtension = "VK_KHR_wayland_surface";

    std::string VkResultName(const VkResult result)
    {
        switch (result)
        {
            case VK_SUCCESS: return "VK_SUCCESS";
            case VK_INCOMPLETE: return "VK_INCOMPLETE";
            case VK_ERROR_OUT_OF_HOST_MEMORY: return "VK_ERROR_OUT_OF_HOST_MEMORY";
            case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
            case VK_ERROR_INITIALIZATION_FAILED: return "VK_ERROR_INITIALIZATION_FAILED";
            case VK_ERROR_LAYER_NOT_PRESENT: return "VK_ERROR_LAYER_NOT_PRESENT";
            case VK_ERROR_EXTENSION_NOT_PRESENT: return "VK_ERROR_EXTENSION_NOT_PRESENT";
            default: return "VkResult(" + std::to_string(static_cast<int>(result)) + ")";
        }
    }

    std::string EnvVar(const char* name)
    {
        const char* value = std::getenv(name);
        if (value == nullptr || value[0] == '\0')
            return "<unset>";

        return value;
    }

    std::vector<VkExtensionProperties> AvailableInstanceExtensions()
    {
        std::uint32_t extensionCount = 0;
        VkResult result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        if (result != VK_SUCCESS)
            throw std::runtime_error("Failed to enumerate Vulkan instance extension count: " + VkResultName(result));

        std::vector<VkExtensionProperties> extensions(extensionCount);
        result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
        if (result != VK_SUCCESS)
            throw std::runtime_error("Failed to enumerate Vulkan instance extensions: " + VkResultName(result));

        return extensions;
    }

    bool HasExtension(const std::vector<VkExtensionProperties>& extensions, const char* extensionName)
    {
        return std::ranges::any_of(extensions, [extensionName](const VkExtensionProperties& extension)
        {
            return std::strcmp(extension.extensionName, extensionName) == 0;
        });
    }

    void AddExtensionIfAvailable(std::vector<const char*>& extensions, const std::vector<VkExtensionProperties>& availableExtensions, const char* extensionName)
    {
        if (!HasExtension(availableExtensions, extensionName))
            return;

        if (std::ranges::find(extensions, extensionName) == extensions.end())
            extensions.emplace_back(extensionName);
    }

    std::string GlfwErrorDescription()
    {
        const char* description = nullptr;
        const int error = glfwGetError(&description);

        if (error == GLFW_NO_ERROR)
            return "no GLFW error reported";

        return std::string(description != nullptr ? description : "unknown GLFW error");
    }

    std::string PlatformName()
    {
        switch (glfwGetPlatform())
        {
            case GLFW_PLATFORM_X11: return "X11";
            case GLFW_PLATFORM_WAYLAND: return "Wayland";
            case GLFW_PLATFORM_NULL: return "Null";
            default: return "Unknown";
        }
    }

    std::string AvailableExtensionList(const std::vector<VkExtensionProperties>& extensions)
    {
        std::ostringstream stream;
        if (extensions.empty())
            return "\n  <none>";

        for (const VkExtensionProperties& extension : extensions)
            stream << "\n  - " << extension.extensionName;
        return stream.str();
    }

    std::string RuntimeDiagnostic(const std::vector<VkExtensionProperties>& availableExtensions)
    {
        std::ostringstream message;
        message
            << "\nGLFW platform: " << PlatformName()
            << "\nGLFW error: " << GlfwErrorDescription()
            << "\nDISPLAY: " << EnvVar("DISPLAY")
            << "\nWAYLAND_DISPLAY: " << EnvVar("WAYLAND_DISPLAY")
            << "\nXDG_SESSION_TYPE: " << EnvVar("XDG_SESSION_TYPE")
            << "\nAvailable Vulkan instance extensions:" << AvailableExtensionList(availableExtensions);
        return message.str();
    }

    std::vector<const char*> BuildPlatformExtensionFallback()
    {
        const std::vector<VkExtensionProperties> availableExtensions = AvailableInstanceExtensions();
        std::vector<const char*> extensions;

        const bool hasSurface = HasExtension(availableExtensions, SurfaceExtension);
        if (hasSurface)
            extensions.emplace_back(SurfaceExtension);

        switch (glfwGetPlatform())
        {
            case GLFW_PLATFORM_X11:
                AddExtensionIfAvailable(extensions, availableExtensions, XcbSurfaceExtension);
                AddExtensionIfAvailable(extensions, availableExtensions, XlibSurfaceExtension);
                break;
            case GLFW_PLATFORM_WAYLAND:
                AddExtensionIfAvailable(extensions, availableExtensions, WaylandSurfaceExtension);
                break;
            default:
                break;
        }

        if (!hasSurface || extensions.size() < 2)
        {
            std::ostringstream message;
            message
                << "GLFW did not provide required Vulkan instance extensions and fallback failed."
                << RuntimeDiagnostic(availableExtensions);
            throw std::runtime_error(message.str());
        }

        std::cerr << "[Vulkan] GLFW did not provide required instance extensions. Using platform fallback for " << PlatformName() << ".\n";
        return extensions;
    }
}

namespace Sunset
{
    VulkanInstance::VulkanInstance(const std::string_view applicationName)
    {
        if (VulkanDebug::ValidationLayersRequested() && !VulkanDebug::ValidationLayersEnabled())
            std::cerr << "[Vulkan] Validation layers requested but not available. Continuing without validation layers.\n";

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = applicationName.data();
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "SunsetEngine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_2;

        const std::vector<const char*> extensions = RequiredExtensions();

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = static_cast<std::uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (VulkanDebug::ValidationLayersEnabled())
        {
            const std::vector<const char*>& validationLayers = VulkanDebug::ValidationLayers();
            createInfo.enabledLayerCount = static_cast<std::uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();

            VulkanDebug::PopulateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = &debugCreateInfo;
        }

        if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS)
            throw std::runtime_error("Failed to create Vulkan instance");

        VulkanDebug::CreateDebugMessenger(m_Instance, m_DebugMessenger);
    }

    VulkanInstance::~VulkanInstance()
    {
        VulkanDebug::DestroyDebugMessenger(m_Instance, m_DebugMessenger);

        if (m_Instance != VK_NULL_HANDLE)
            vkDestroyInstance(m_Instance, nullptr);
    }

    VkInstance VulkanInstance::Get() const
    {
        return m_Instance;
    }

    std::vector<const char*> VulkanInstance::RequiredExtensions()
    {
        if (glfwVulkanSupported() != GLFW_TRUE)
            throw std::runtime_error("GLFW reports Vulkan is not supported. Check that the Vulkan loader and a GPU ICD/driver are installed.");

        std::uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        std::vector<const char*> extensions;
        if (glfwExtensions != nullptr && glfwExtensionCount != 0)
            extensions = {glfwExtensions, glfwExtensions + glfwExtensionCount};
        else
        {
            std::cerr << "[Vulkan] glfwGetRequiredInstanceExtensions returned no extensions. Trying platform fallback.\n";
            extensions = BuildPlatformExtensionFallback();
        }

        if (VulkanDebug::ValidationLayersEnabled())
            extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        return extensions;
    }
}
