//
// Created by Codex on 30/06/2026.
//

#pragma once

#include "FrameBufferSpecification.h"

#include <cstdint>
#include <vector>

namespace Sunset
{
    class FrameBuffer
    {
    public:
        explicit FrameBuffer(const FrameBufferSpecification& specification);
        ~FrameBuffer();

        FrameBuffer(const FrameBuffer&) = delete;
        FrameBuffer& operator=(const FrameBuffer&) = delete;

        FrameBuffer(FrameBuffer&& other) noexcept;
        FrameBuffer& operator=(FrameBuffer&& other) noexcept;

        void Bind() const;
        static void Unbind();

        void Resize(int width, int height);
        void Clear(const glm::vec4& color = glm::vec4(0.0f)) const;
        void Clear(ClearFlags flags, const glm::vec4& color = glm::vec4(0.0f), float depth = 1.0f, int stencil = 0) const;
        void ClearColor(std::uint32_t attachmentIndex, const glm::vec4& color) const;
        void ClearDepth(float depth = 1.0f) const;
        void ClearStencil(int stencil = 0) const;
        void BlitTo(FrameBuffer& target, ClearFlags mask) const;

        [[nodiscard]] std::uint32_t GetColorAttachment(std::uint32_t index = 0) const;
        [[nodiscard]] std::uint32_t GetDepthAttachment() const;
        [[nodiscard]] bool HasDepthAttachment() const;
        [[nodiscard]] std::uint32_t GetColorAttachmentCount() const;
        [[nodiscard]] std::uint32_t GetId() const;
        [[nodiscard]] int GetWidth() const;
        [[nodiscard]] int GetHeight() const;
        [[nodiscard]] int GetSamples() const;
        [[nodiscard]] bool IsMultisampled() const;
        [[nodiscard]] bool IsValid() const;

    private:
        void Invalidate();
        void Release();

        FrameBufferSpecification m_Specification;
        std::uint32_t m_Id = 0;
        std::vector<std::uint32_t> m_ColorAttachments;
        std::uint32_t m_DepthAttachment = 0;
    };
}
