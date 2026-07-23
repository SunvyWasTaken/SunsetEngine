//
// Created by Codex on 30/06/2026.
//

#include "FrameBuffer.h"

#include <glad/glad.h>

namespace
{
    bool IsDepthFormat(const Sunset::FrameBufferTextureFormat format)
    {
        switch (format)
        {
            case Sunset::FrameBufferTextureFormat::Depth24Stencil8:
            case Sunset::FrameBufferTextureFormat::Depth32F:
                return true;
            default:
                return false;
        }
    }

    bool HasStencil(const Sunset::FrameBufferTextureFormat format)
    {
        return format == Sunset::FrameBufferTextureFormat::Depth24Stencil8;
    }

    GLenum ToGLInternalFormat(const Sunset::FrameBufferTextureFormat format)
    {
        switch (format)
        {
            case Sunset::FrameBufferTextureFormat::RGBA8:           return GL_RGBA8;
            case Sunset::FrameBufferTextureFormat::RGBA16F:         return GL_RGBA16F;
            case Sunset::FrameBufferTextureFormat::R32F:            return GL_R32F;
            case Sunset::FrameBufferTextureFormat::Depth24Stencil8: return GL_DEPTH24_STENCIL8;
            case Sunset::FrameBufferTextureFormat::Depth32F:        return GL_DEPTH_COMPONENT32F;
            case Sunset::FrameBufferTextureFormat::None:            return GL_NONE;
        }

        return GL_NONE;
    }

    GLenum ToGLDataFormat(const Sunset::FrameBufferTextureFormat format)
    {
        switch (format)
        {
            case Sunset::FrameBufferTextureFormat::RGBA8:
            case Sunset::FrameBufferTextureFormat::RGBA16F:
                return GL_RGBA;
            case Sunset::FrameBufferTextureFormat::R32F:
                return GL_RED;
            case Sunset::FrameBufferTextureFormat::Depth24Stencil8:
                return GL_DEPTH_STENCIL;
            case Sunset::FrameBufferTextureFormat::Depth32F:
                return GL_DEPTH_COMPONENT;
            case Sunset::FrameBufferTextureFormat::None:
                return GL_NONE;
        }

        return GL_NONE;
    }

    GLenum ToGLDataType(const Sunset::FrameBufferTextureFormat format)
    {
        switch (format)
        {
            case Sunset::FrameBufferTextureFormat::RGBA8:
                return GL_UNSIGNED_BYTE;
            case Sunset::FrameBufferTextureFormat::RGBA16F:
            case Sunset::FrameBufferTextureFormat::R32F:
            case Sunset::FrameBufferTextureFormat::Depth32F:
                return GL_FLOAT;
            case Sunset::FrameBufferTextureFormat::Depth24Stencil8:
                return GL_UNSIGNED_INT_24_8;
            case Sunset::FrameBufferTextureFormat::None:
                return GL_NONE;
        }

        return GL_NONE;
    }

    GLenum ToGLDepthAttachment(const Sunset::FrameBufferTextureFormat format)
    {
        return HasStencil(format) ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT;
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
        other.m_Id = 0;
        other.m_DepthAttachment = 0;
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

        other.m_Id = 0;
        other.m_DepthAttachment = 0;
        return *this;
    }

    void FrameBuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_Id);
        glViewport(0, 0, m_Specification.width, m_Specification.height);
    }

    void FrameBuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
        Bind();
        glClearColor(color.r, color.g, color.b, color.a);
        GLbitfield clearFlags = 0;

        if (!m_ColorAttachments.empty())
            clearFlags |= GL_COLOR_BUFFER_BIT;

        if (m_DepthAttachment != 0)
        {
            clearFlags |= GL_DEPTH_BUFFER_BIT;

            for (const auto& attachment : m_Specification.attachments)
            {
                if (HasStencil(attachment.format))
                {
                    clearFlags |= GL_STENCIL_BUFFER_BIT;
                    break;
                }
            }
        }

        if (clearFlags != 0)
            glClear(clearFlags);
    }

    std::uint32_t FrameBuffer::GetColorAttachment(const std::uint32_t index) const
    {
        return index < m_ColorAttachments.size() ? m_ColorAttachments[index] : 0;
    }

    std::uint32_t FrameBuffer::GetDepthAttachment() const
    {
        return m_DepthAttachment;
    }

    std::uint32_t FrameBuffer::GetColorAttachmentCount() const
    {
        return static_cast<std::uint32_t>(m_ColorAttachments.size());
    }

    std::uint32_t FrameBuffer::GetId() const
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

    bool FrameBuffer::IsValid() const
    {
        if (m_Id == 0)
            return false;

        GLint previousFrameBuffer = 0;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFrameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_Id);
        const bool valid = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
        glBindFramebuffer(GL_FRAMEBUFFER, previousFrameBuffer);
        return valid;
    }

    void FrameBuffer::Invalidate()
    {
        Release();

        glGenFramebuffers(1, &m_Id);
        glBindFramebuffer(GL_FRAMEBUFFER, m_Id);

        GLint maxColorAttachments = 0;
        glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);

        std::vector<GLenum> drawBuffers;
        for (const auto& attachment : m_Specification.attachments)
        {
            if (attachment.format == FrameBufferTextureFormat::None)
                continue;

            if (!IsDepthFormat(attachment.format) && static_cast<GLint>(m_ColorAttachments.size()) >= maxColorAttachments)
            {
                LOG("Engine", error, "Framebuffer {} has more color attachments than the GPU supports", m_Id)
                continue;
            }

            std::uint32_t texture = 0;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                ToGLInternalFormat(attachment.format),
                m_Specification.width,
                m_Specification.height,
                0,
                ToGLDataFormat(attachment.format),
                ToGLDataType(attachment.format),
                nullptr);

            if (IsDepthFormat(attachment.format))
            {
                if (m_DepthAttachment != 0)
                {
                    LOG("Engine", error, "Framebuffer {} has more than one depth attachment", m_Id)
                    glDeleteTextures(1, &texture);
                    continue;
                }

                m_DepthAttachment = texture;
                glFramebufferTexture2D(GL_FRAMEBUFFER, ToGLDepthAttachment(attachment.format), GL_TEXTURE_2D, m_DepthAttachment, 0);
            }
            else
            {
                const std::uint32_t colorAttachmentIndex = static_cast<std::uint32_t>(m_ColorAttachments.size());
                m_ColorAttachments.emplace_back(texture);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttachmentIndex, GL_TEXTURE_2D, texture, 0);
                drawBuffers.emplace_back(GL_COLOR_ATTACHMENT0 + colorAttachmentIndex);
            }
        }

        if (!drawBuffers.empty())
            glDrawBuffers(static_cast<GLsizei>(drawBuffers.size()), drawBuffers.data());
        else
        {
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            LOG("Engine", error, "Framebuffer {} is incomplete", m_Id)

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::Release()
    {
        if (m_Id != 0)
            glDeleteFramebuffers(1, &m_Id);

        if (!m_ColorAttachments.empty())
            glDeleteTextures(static_cast<GLsizei>(m_ColorAttachments.size()), m_ColorAttachments.data());

        if (m_DepthAttachment != 0)
            glDeleteTextures(1, &m_DepthAttachment);

        m_Id = 0;
        m_ColorAttachments.clear();
        m_DepthAttachment = 0;
    }
}
