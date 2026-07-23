//
// Created by Codex on 23/07/2026.
//

#include "VulkanSurface.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>

namespace Sunset
{
    VulkanSurface::VulkanSurface(const VkInstance instance, void* nativeWindow)
        : m_Instance(instance)
    {
        auto* window = static_cast<GLFWwindow*>(nativeWindow);
        if (window == nullptr)
            throw std::runtime_error("Vulkan requires a GLFW native window");

        if (glfwCreateWindowSurface(m_Instance, window, nullptr, &m_Surface) != VK_SUCCESS)
            throw std::runtime_error("Failed to create Vulkan window surface");
    }

    VulkanSurface::~VulkanSurface()
    {
        if (m_Surface != VK_NULL_HANDLE)
            vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
    }

    VkSurfaceKHR VulkanSurface::Get() const
    {
        return m_Surface;
    }
}
