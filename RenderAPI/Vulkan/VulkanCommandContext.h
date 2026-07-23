//
// Created by Codex on 23/07/2026.
//

#pragma once

#include "VulkanDevice.h"
#include "VulkanSwapchain.h"

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

#include <vector>

namespace Sunset
{
    class VulkanCommandContext
    {
    public:
        explicit VulkanCommandContext(VulkanDevice& device, VulkanSwapchain& swapchain);
        ~VulkanCommandContext();

        VulkanCommandContext(const VulkanCommandContext&) = delete;
        VulkanCommandContext& operator=(const VulkanCommandContext&) = delete;
        VulkanCommandContext(VulkanCommandContext&&) = delete;
        VulkanCommandContext& operator=(VulkanCommandContext&&) = delete;

        void SetClearColor(const glm::vec4& clearColor);
        void BeginFrame();
        void EndFrame();

    private:
        static constexpr std::uint32_t MaxFramesInFlight = 2;

        void CreateCommandPool();
        void CreateCommandBuffers();
        void CreateSyncObjects();
        void Destroy();

        VulkanDevice& m_Device;
        VulkanSwapchain& m_Swapchain;
        VkCommandPool m_CommandPool = VK_NULL_HANDLE;
        std::vector<VkCommandBuffer> m_CommandBuffers;
        std::vector<VkSemaphore> m_ImageAvailableSemaphores;
        std::vector<VkSemaphore> m_RenderFinishedSemaphores;
        std::vector<VkFence> m_InFlightFences;
        glm::vec4 m_ClearColor{0.05f, 0.07f, 0.09f, 1.0f};
        std::uint32_t m_CurrentFrame = 0;
        std::uint32_t m_CurrentImageIndex = 0;
        bool m_FrameStarted = false;
    };
}
