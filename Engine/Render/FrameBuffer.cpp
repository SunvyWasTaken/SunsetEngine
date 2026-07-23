//
// Created by Codex on 30/06/2026.
//

#include "FrameBuffer.h"

#include "Render/Core/RenderAPI.h"
#include "Render/Core/RenderCommand.h"

namespace
{
    bool HasStencil(const Sunset::FrameBufferTextureFormat format)
    {
        return format == Sunset::FrameBufferTextureFormat::Depth24Stencil8;
    }
}

namespace Sunset
{
    FrameBuffer::FrameBuffer(const FrameBufferSpecification& specification)
        : m_Specification(specification)
    {
        Invalidate();
    }

    FrameBuffer::~FrameBuffer()
    {
        Release();
    }

    FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept
        : m_Specification(other.m_Specification)
        , m_Id(other.m_Id)
        , m_ColorAttachments(std::move(other.m_ColorAttachments))
        , m_DepthAttachment(other.m_DepthAttachment)
    {
        other.m_Id = {};
        other.m_DepthAttachment = {};
    }

    FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other) noexcept
    {
        if (this == &other)
            return *this;

        Release();
        m_Specification = other.m_Specification;
        m_Id = other.m_Id;
        m_ColorAttachments = std::move(other.m_ColorAttachments);
        m_DepthAttachment = other.m_DepthAttachment;

        other.m_Id = {};
        other.m_DepthAttachment = {};
        return *this;
    }

    void FrameBuffer::Bind() const
    {
        RenderCommand::BindFrameBuffer(m_Id, { m_Specification.width, m_Specification.height });
    }

    void FrameBuffer::Unbind()
    {
        RenderCommand::UnbindFrameBuffer();
    }

    void FrameBuffer::Resize(const int width, const int height)
    {
        if (width <= 0 || height <= 0)
            return;

        if (m_Specification.width == width && m_Specification.height == height)
            return;

        m_Specification.width = width;
        m_Specification.height = height;
        Invalidate();
    }

    void FrameBuffer::Clear(const glm::vec4& color) const
    {
        Clear(ClearFlags::Color | ClearFlags::Depth | ClearFlags::Stencil, color);
    }

    void FrameBuffer::Clear(const ClearFlags flags, const glm::vec4& color, const float depth, const int stencil) const
    {
        RenderCommand::ClearFrameBuffer(*this, flags, color, depth, stencil);
    }

    void FrameBuffer::ClearColor(const std::uint32_t attachmentIndex, const glm::vec4& color) const
    {
        RenderCommand::ClearFrameBufferColor(*this, attachmentIndex, color);
    }

    void FrameBuffer::ClearDepth(const float depth) const
    {
        RenderCommand::ClearFrameBufferDepth(*this, depth);
    }

    void FrameBuffer::ClearStencil(const int stencil) const
    {
        RenderCommand::ClearFrameBufferStencil(*this, stencil);
    }

    void FrameBuffer::BlitTo(FrameBuffer& target, const ClearFlags mask) const
    {
        RenderCommand::BlitFrameBuffer(*this, target, mask);
    }

    TextureHandle FrameBuffer::GetColorAttachment(const std::uint32_t index) const
    {
        return index < m_ColorAttachments.size() ? m_ColorAttachments[index] : TextureHandle{};
    }

    TextureHandle FrameBuffer::GetDepthAttachment() const
    {
        return m_DepthAttachment;
    }

    bool FrameBuffer::HasDepthAttachment() const
    {
        return static_cast<bool>(m_DepthAttachment);
    }

    std::uint32_t FrameBuffer::GetColorAttachmentCount() const
    {
        return static_cast<std::uint32_t>(m_ColorAttachments.size());
    }

    FrameBufferHandle FrameBuffer::GetId() const
    {
        return m_Id;
    }

    int FrameBuffer::GetWidth() const
    {
        return m_Specification.width;
    }

    int FrameBuffer::GetHeight() const
    {
        return m_Specification.height;
    }

    int FrameBuffer::GetSamples() const
    {
        return m_Specification.samples;
    }

    bool FrameBuffer::IsMultisampled() const
    {
        return m_Specification.samples > 1;
    }

    bool FrameBuffer::HasStencilAttachment() const
    {
        for (const auto& attachment : m_Specification.attachments)
        {
            if (HasStencil(attachment.format))
                return true;
        }

        return false;
    }

    const FrameBufferSpecification& FrameBuffer::GetSpecification() const
    {
        return m_Specification;
    }

    bool FrameBuffer::IsValid() const
    {
        return static_cast<bool>(m_Id) && RenderCommand::IsFrameBufferValid(m_Id);
    }

    void FrameBuffer::Invalidate()
    {
        Release();

        FrameBufferCreateResult result = RenderCommand::CreateFrameBuffer(m_Specification);
        m_Id = result.id;
        m_ColorAttachments = std::move(result.colorAttachments);
        m_DepthAttachment = result.depthAttachment;
    }

    void FrameBuffer::Release()
    {
        RenderCommand::DestroyFrameBuffer(m_Id, m_ColorAttachments, m_DepthAttachment);

        m_Id = {};
        m_ColorAttachments.clear();
        m_DepthAttachment = {};
    }
}
