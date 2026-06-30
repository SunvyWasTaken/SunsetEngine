//
// Created by Codex on 30/06/2026.
//

#include "FrameBuffer.h"

#include <glad/glad.h>

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
        , m_ColorAttachment(other.m_ColorAttachment)
        , m_DepthAttachment(other.m_DepthAttachment)
    {
        other.m_Id = 0;
        other.m_ColorAttachment = 0;
        other.m_DepthAttachment = 0;
    }

    FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other) noexcept
    {
        if (this == &other)
            return *this;

        Release();
        m_Specification = other.m_Specification;
        m_Id = other.m_Id;
        m_ColorAttachment = other.m_ColorAttachment;
        m_DepthAttachment = other.m_DepthAttachment;

        other.m_Id = 0;
        other.m_ColorAttachment = 0;
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
        glClear(GL_COLOR_BUFFER_BIT | (m_Specification.useDepth ? GL_DEPTH_BUFFER_BIT : 0));
    }

    std::uint32_t FrameBuffer::GetColorAttachment() const
    {
        return m_ColorAttachment;
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

        glGenTextures(1, &m_ColorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.width, m_Specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

        if (m_Specification.useDepth)
        {
            glGenTextures(1, &m_DepthAttachment);
            glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.width, m_Specification.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);
        }

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            LOG("Engine", error, "Framebuffer {} is incomplete", m_Id)

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::Release()
    {
        glDeleteFramebuffers(1, &m_Id);
        glDeleteTextures(1, &m_ColorAttachment);
        glDeleteTextures(1, &m_DepthAttachment);

        m_Id = 0;
        m_ColorAttachment = 0;
        m_DepthAttachment = 0;
    }
}
