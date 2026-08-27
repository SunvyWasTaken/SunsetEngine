//
// Created by sunvy on 26/08/2026.
//

#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Sunset
{
    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification &specification)
        : m_RenderID(0)
        , spec(specification)
    {
        Invalidate();
    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        LOG("OpenGL", trace, "OpenGL Framebuffer {} destroy", m_RenderID);
        glDeleteFramebuffers(1, &m_RenderID);
    }

    std::uint32_t OpenGLFramebuffer::GetColorAttachmentRenderID() const
    {
        return m_ColorAttachment;
    }

    void OpenGLFramebuffer::Invalidate()
    {
        glCreateFramebuffers(1, &m_RenderID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RenderID);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, spec.Width, spec.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
        glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
        // glTexStorage2D(GL_TEXTURE_2D, 0, GL_DEPTH_STENCIL, spec.Width, spec.Height);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, spec.Width, spec.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            throw std::runtime_error("Framebuffer is not complete");

        LOG("OpenGL", trace, "OpenGL Framebuffer {} created", m_RenderID);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RenderID);
    }

    void OpenGLFramebuffer::UnBind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
} // Sunset