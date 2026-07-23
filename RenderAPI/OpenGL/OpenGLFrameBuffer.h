//
// Created by Codex on 23/07/2026.
//

#pragma once

#include "Render/Core/RenderAPI.h"

namespace Sunset
{
    class FrameBuffer;
}

namespace Sunset::OpenGLFrameBuffer
{
    [[nodiscard]] FrameBufferCreateResult Create(const FrameBufferSpecification& specification);
    void Destroy(std::uint32_t framebuffer, const std::vector<std::uint32_t>& colorAttachments, std::uint32_t depthAttachment);
    void Bind(std::uint32_t framebuffer, const glm::ivec2& size);
    void Unbind();
    void Clear(const FrameBuffer& target, ClearFlags flags, const glm::vec4& color, float depth, int stencil);
    void ClearColor(const FrameBuffer& target, std::uint32_t attachmentIndex, const glm::vec4& color);
    void ClearDepth(const FrameBuffer& target, float depth);
    void ClearStencil(const FrameBuffer& target, int stencil);
    void Blit(const FrameBuffer& source, FrameBuffer& target, ClearFlags mask);
    [[nodiscard]] bool IsValid(std::uint32_t framebuffer);
}
