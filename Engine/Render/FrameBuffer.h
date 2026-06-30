//
// Created by Codex on 30/06/2026.
//

#pragma once

namespace Sunset
{
    struct FrameBufferSpecification
    {
        int width = 1;
        int height = 1;
        bool useDepth = true;
    };

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

        [[nodiscard]] std::uint32_t GetColorAttachment() const;
        [[nodiscard]] std::uint32_t GetId() const;
        [[nodiscard]] int GetWidth() const;
        [[nodiscard]] int GetHeight() const;
        [[nodiscard]] bool IsValid() const;

    private:
        void Invalidate();
        void Release();

        FrameBufferSpecification m_Specification;
        std::uint32_t m_Id = 0;
        std::uint32_t m_ColorAttachment = 0;
        std::uint32_t m_DepthAttachment = 0;
    };
}
