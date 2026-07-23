//
// Created by Codex on 23/07/2026.
//

#include "OpenGLFrameBuffer.h"

#include "Render/FrameBuffer.h"

#include <glad/glad.h>

namespace
{
    std::vector<std::uint32_t> ToOpenGLTextureIds(const std::vector<Sunset::TextureHandle>& handles)
    {
        std::vector<std::uint32_t> ids;
        ids.reserve(handles.size());

        for (const Sunset::TextureHandle handle : handles)
            ids.emplace_back(handle.id);

        return ids;
    }

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

    GLenum ToGLTextureFilter(const Sunset::TextureFilter filter)
    {
        switch (filter)
        {
            case Sunset::TextureFilter::Nearest: return GL_NEAREST;
            case Sunset::TextureFilter::Linear:  return GL_LINEAR;
        }

        return GL_NEAREST;
    }

    GLenum ToGLTextureWrap(const Sunset::TextureWrap wrap)
    {
        switch (wrap)
        {
            case Sunset::TextureWrap::ClampToEdge:   return GL_CLAMP_TO_EDGE;
            case Sunset::TextureWrap::ClampToBorder: return GL_CLAMP_TO_BORDER;
            case Sunset::TextureWrap::Repeat:        return GL_REPEAT;
        }

        return GL_CLAMP_TO_EDGE;
    }

    GLenum TextureTarget(const int samples)
    {
        return samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }

    void ApplyTextureParameters(const Sunset::FrameBufferTextureSpecification& attachment)
    {
        const GLenum filter = ToGLTextureFilter(attachment.filter);
        const GLenum wrap = ToGLTextureWrap(attachment.wrap);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

        if (attachment.wrap == Sunset::TextureWrap::ClampToBorder)
        {
            const float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        }
    }

    GLbitfield ToGLClearFlags(const Sunset::FrameBuffer& frameBuffer, const Sunset::ClearFlags flags)
    {
        GLbitfield clearFlags = 0;

        if (Sunset::HasClearFlag(flags, Sunset::ClearFlags::Color) && frameBuffer.GetColorAttachmentCount() > 0)
            clearFlags |= GL_COLOR_BUFFER_BIT;

        if (Sunset::HasClearFlag(flags, Sunset::ClearFlags::Depth) && frameBuffer.HasDepthAttachment())
            clearFlags |= GL_DEPTH_BUFFER_BIT;

        if (Sunset::HasClearFlag(flags, Sunset::ClearFlags::Stencil) && frameBuffer.HasStencilAttachment())
            clearFlags |= GL_STENCIL_BUFFER_BIT;

        return clearFlags;
    }
}

namespace Sunset::OpenGLFrameBuffer
{
    FrameBufferCreateResult Create(const FrameBufferSpecification& specification)
    {
        FrameBufferSpecification spec = specification;
        if (spec.samples < 1)
            spec.samples = 1;

        if (spec.samples > 1)
        {
            GLint maxSamples = 1;
            glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
            if (spec.samples > maxSamples)
            {
                LOG("OpenGL", error, "Framebuffer requested {} samples but this GPU supports only {}", spec.samples, maxSamples)
                spec.samples = maxSamples;
            }
        }

        FrameBufferCreateResult result;
        glGenFramebuffers(1, &result.id.id);
        glBindFramebuffer(GL_FRAMEBUFFER, result.id.id);

        GLint maxColorAttachments = 0;
        glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);

        std::vector<GLenum> drawBuffers;
        const GLenum textureTarget = TextureTarget(spec.samples);
        for (const FrameBufferTextureSpecification& attachment : spec.attachments)
        {
            if (attachment.format == FrameBufferTextureFormat::None)
                continue;

            if (!IsDepthFormat(attachment.format) && static_cast<GLint>(result.colorAttachments.size()) >= maxColorAttachments)
            {
                LOG("OpenGL", error, "Framebuffer {} has more color attachments than the GPU supports", result.id.id)
                continue;
            }

            std::uint32_t texture = 0;
            glGenTextures(1, &texture);
            glBindTexture(textureTarget, texture);

            if (spec.samples > 1)
            {
                glTexImage2DMultisample(
                    textureTarget,
                    spec.samples,
                    ToGLInternalFormat(attachment.format),
                    spec.width,
                    spec.height,
                    GL_TRUE);
            }
            else
            {
                ApplyTextureParameters(attachment);
                glTexImage2D(
                    textureTarget,
                    0,
                    ToGLInternalFormat(attachment.format),
                    spec.width,
                    spec.height,
                    0,
                    ToGLDataFormat(attachment.format),
                    ToGLDataType(attachment.format),
                    nullptr);
            }

            if (IsDepthFormat(attachment.format))
            {
                if (result.depthAttachment)
                {
                    LOG("OpenGL", error, "Framebuffer {} has more than one depth attachment", result.id.id)
                    glDeleteTextures(1, &texture);
                    continue;
                }

                result.depthAttachment = TextureHandle{texture};
                glFramebufferTexture2D(GL_FRAMEBUFFER, ToGLDepthAttachment(attachment.format), textureTarget, result.depthAttachment.id, 0);
            }
            else
            {
                const std::uint32_t colorAttachmentIndex = static_cast<std::uint32_t>(result.colorAttachments.size());
                result.colorAttachments.emplace_back(TextureHandle{texture});
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttachmentIndex, textureTarget, texture, 0);
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
            LOG("OpenGL", error, "Framebuffer {} is incomplete", result.id.id)

        glBindTexture(textureTarget, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return result;
    }

    void Destroy(const FrameBufferHandle framebuffer, const std::vector<TextureHandle>& colorAttachments, const TextureHandle depthAttachment)
    {
        if (framebuffer)
            glDeleteFramebuffers(1, &framebuffer.id);

        if (!colorAttachments.empty())
        {
            const std::vector<std::uint32_t> colorAttachmentIds = ToOpenGLTextureIds(colorAttachments);
            glDeleteTextures(static_cast<GLsizei>(colorAttachmentIds.size()), colorAttachmentIds.data());
        }

        if (depthAttachment)
            glDeleteTextures(1, &depthAttachment.id);
    }

    void Bind(const std::uint32_t framebuffer, const glm::ivec2& size)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glViewport(0, 0, size.x, size.y);
    }

    void Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Clear(const FrameBuffer& target, const ClearFlags flags, const glm::vec4& color, const float depth, const int stencil)
    {
        target.Bind();
        glClearColor(color.r, color.g, color.b, color.a);
        glClearDepth(depth);
        glClearStencil(stencil);

        const GLbitfield clearFlags = ToGLClearFlags(target, flags);
        if (clearFlags != 0)
            glClear(clearFlags);
    }

    void ClearColor(const FrameBuffer& target, const std::uint32_t attachmentIndex, const glm::vec4& color)
    {
        if (attachmentIndex >= target.GetColorAttachmentCount())
            return;

        target.Bind();
        const float values[] = { color.r, color.g, color.b, color.a };
        glClearBufferfv(GL_COLOR, static_cast<GLint>(attachmentIndex), values);
    }

    void ClearDepth(const FrameBuffer& target, const float depth)
    {
        if (!target.HasDepthAttachment())
            return;

        target.Bind();
        glClearBufferfv(GL_DEPTH, 0, &depth);
    }

    void ClearStencil(const FrameBuffer& target, const int stencil)
    {
        if (!target.HasStencilAttachment())
            return;

        target.Bind();
        glClearBufferiv(GL_STENCIL, 0, &stencil);
    }

    void Blit(const FrameBuffer& source, FrameBuffer& target, const ClearFlags mask)
    {
        GLbitfield blitMask = 0;

        if (HasClearFlag(mask, ClearFlags::Color) && source.GetColorAttachmentCount() > 0 && target.GetColorAttachmentCount() > 0)
            blitMask |= GL_COLOR_BUFFER_BIT;

        if (HasClearFlag(mask, ClearFlags::Depth) && source.HasDepthAttachment() && target.HasDepthAttachment())
            blitMask |= GL_DEPTH_BUFFER_BIT;

        if (HasClearFlag(mask, ClearFlags::Stencil) && source.HasStencilAttachment() && target.HasStencilAttachment())
            blitMask |= GL_STENCIL_BUFFER_BIT;

        if (blitMask == 0)
            return;

        GLint previousReadFrameBuffer = 0;
        GLint previousDrawFrameBuffer = 0;
        glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &previousReadFrameBuffer);
        glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &previousDrawFrameBuffer);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, source.GetId().id);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target.GetId().id);
        glBlitFramebuffer(
            0,
            0,
            source.GetWidth(),
            source.GetHeight(),
            0,
            0,
            target.GetWidth(),
            target.GetHeight(),
            blitMask,
            GL_NEAREST);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, previousReadFrameBuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, previousDrawFrameBuffer);
    }

    bool IsValid(const std::uint32_t framebuffer)
    {
        if (framebuffer == 0)
            return false;

        GLint previousFrameBuffer = 0;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFrameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        const bool valid = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
        glBindFramebuffer(GL_FRAMEBUFFER, previousFrameBuffer);
        return valid;
    }
}
