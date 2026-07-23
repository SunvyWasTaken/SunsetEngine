//
// Created by Codex on 23/07/2026.
//

#include "VulkanCommandContext.h"

#include <array>
#include <stdexcept>

namespace Sunset
{
    VulkanCommandContext::VulkanCommandContext(VulkanDevice& device, VulkanSwapchain& swapchain)
        : m_Device(device)
        , m_Swapchain(swapchain)
    {
        CreateCommandPool();
        CreateCommandBuffers();
        CreateSyncObjects();
    }

    VulkanCommandContext::~VulkanCommandContext()
    {
        Destroy();
    }

    void VulkanCommandContext::SetClearColor(const glm::vec4& clearColor)
    {
        m_ClearColor = clearColor;
    }

    void VulkanCommandContext::BeginFrame()
    {
        if (m_FrameStarted)
            return;

        const VkDevice device = m_Device.GetDevice();
        vkWaitForFences(device, 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);

        const VkResult acquireResult = vkAcquireNextImageKHR(
            device,
            m_Swapchain.Get(),
            UINT64_MAX,
            m_ImageAvailableSemaphores[m_CurrentFrame],
            VK_NULL_HANDLE,
            &m_CurrentImageIndex);

        if (acquireResult != VK_SUCCESS && acquireResult != VK_SUBOPTIMAL_KHR)
            throw std::runtime_error("Failed to acquire Vulkan swapchain image");

        vkResetFences(device, 1, &m_InFlightFences[m_CurrentFrame]);
        vkResetCommandBuffer(m_CommandBuffers[m_CurrentFrame], 0);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(m_CommandBuffers[m_CurrentFrame], &beginInfo) != VK_SUCCESS)
            throw std::runtime_error("Failed to begin Vulkan command buffer");

        std::array<VkClearValue, 1> clearValues{};
        clearValues[0].color = {{m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a}};

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_Swapchain.GetRenderPass();
        renderPassInfo.framebuffer = m_Swapchain.GetFramebuffer(m_CurrentImageIndex);
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = m_Swapchain.GetExtent();
        renderPassInfo.clearValueCount = static_cast<std::uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(m_CommandBuffers[m_CurrentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        m_FrameStarted = true;
    }

    void VulkanCommandContext::EndFrame()
    {
        if (!m_FrameStarted)
            return;

        VkCommandBuffer commandBuffer = m_CommandBuffers[m_CurrentFrame];
        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
            throw std::runtime_error("Failed to record Vulkan command buffer");

        const std::array waitSemaphores = {
            m_ImageAvailableSemaphores[m_CurrentFrame]
        };
        const std::array signalSemaphores = {
            m_RenderFinishedSemaphores[m_CurrentFrame]
        };
        const std::array<VkPipelineStageFlags, 1> waitStages = {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
        };

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = static_cast<std::uint32_t>(waitSemaphores.size());
        submitInfo.pWaitSemaphores = waitSemaphores.data();
        submitInfo.pWaitDstStageMask = waitStages.data();
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;
        submitInfo.signalSemaphoreCount = static_cast<std::uint32_t>(signalSemaphores.size());
        submitInfo.pSignalSemaphores = signalSemaphores.data();

        if (vkQueueSubmit(m_Device.GetGraphicsQueue(), 1, &submitInfo, m_InFlightFences[m_CurrentFrame]) != VK_SUCCESS)
            throw std::runtime_error("Failed to submit Vulkan draw command buffer");

        const std::array swapchains = {
            m_Swapchain.Get()
        };

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = static_cast<std::uint32_t>(signalSemaphores.size());
        presentInfo.pWaitSemaphores = signalSemaphores.data();
        presentInfo.swapchainCount = static_cast<std::uint32_t>(swapchains.size());
        presentInfo.pSwapchains = swapchains.data();
        presentInfo.pImageIndices = &m_CurrentImageIndex;

        const VkResult presentResult = vkQueuePresentKHR(m_Device.GetPresentQueue(), &presentInfo);
        if (presentResult != VK_SUCCESS && presentResult != VK_SUBOPTIMAL_KHR)
            throw std::runtime_error("Failed to present Vulkan swapchain image");

        m_CurrentFrame = (m_CurrentFrame + 1) % MaxFramesInFlight;
        m_FrameStarted = false;
    }

    void VulkanCommandContext::CreateCommandPool()
    {
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = m_Device.GetQueueFamilies().graphicsFamily.value();

        if (vkCreateCommandPool(m_Device.GetDevice(), &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS)
            throw std::runtime_error("Failed to create Vulkan command pool");
    }

    void VulkanCommandContext::CreateCommandBuffers()
    {
        m_CommandBuffers.resize(MaxFramesInFlight);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_CommandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = static_cast<std::uint32_t>(m_CommandBuffers.size());

        if (vkAllocateCommandBuffers(m_Device.GetDevice(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
            throw std::runtime_error("Failed to allocate Vulkan command buffers");
    }

    void VulkanCommandContext::CreateSyncObjects()
    {
        m_ImageAvailableSemaphores.resize(MaxFramesInFlight);
        m_RenderFinishedSemaphores.resize(MaxFramesInFlight);
        m_InFlightFences.resize(MaxFramesInFlight);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (std::uint32_t index = 0; index < MaxFramesInFlight; ++index)
        {
            if (vkCreateSemaphore(m_Device.GetDevice(), &semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[index]) != VK_SUCCESS ||
                vkCreateSemaphore(m_Device.GetDevice(), &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[index]) != VK_SUCCESS ||
                vkCreateFence(m_Device.GetDevice(), &fenceInfo, nullptr, &m_InFlightFences[index]) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create Vulkan frame sync objects");
            }
        }
    }

    void VulkanCommandContext::Destroy()
    {
        const VkDevice device = m_Device.GetDevice();

        for (const VkFence fence : m_InFlightFences)
            vkDestroyFence(device, fence, nullptr);
        m_InFlightFences.clear();

        for (const VkSemaphore semaphore : m_RenderFinishedSemaphores)
            vkDestroySemaphore(device, semaphore, nullptr);
        m_RenderFinishedSemaphores.clear();

        for (const VkSemaphore semaphore : m_ImageAvailableSemaphores)
            vkDestroySemaphore(device, semaphore, nullptr);
        m_ImageAvailableSemaphores.clear();

        if (m_CommandPool != VK_NULL_HANDLE)
            vkDestroyCommandPool(device, m_CommandPool, nullptr);
        m_CommandPool = VK_NULL_HANDLE;
    }
}
