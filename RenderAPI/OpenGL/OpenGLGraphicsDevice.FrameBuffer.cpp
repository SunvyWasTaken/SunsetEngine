//
// Created by Codex on 23/07/2026.
//

#include "OpenGLGraphicsDevice.h"

#include "OpenGLFrameBuffer.h"

namespace Sunset
{
    FrameBufferCreateResult OpenGLGraphicsDevice::CreateFrameBuffer(const FrameBufferSpecification& specification)
    {
        return OpenGLFrameBuffer::Create(specification);
    }

    void OpenGLGraphicsDevice::DestroyFrameBuffer(const std::uint32_t framebuffer, const std::vector<std::uint32_t>& colorAttachments, const std::uint32_t depthAttachment)
    {
        OpenGLFrameBuffer::Destroy(framebuffer, colorAttachments, depthAttachment);
    }

    void OpenGLGraphicsDevice::BindFrameBuffer(const std::uint32_t framebuffer, const glm::ivec2& size)
    {
        OpenGLFrameBuffer::Bind(framebuffer, size);
    }

    void OpenGLGraphicsDevice::UnbindFrameBuffer()
    {
        OpenGLFrameBuffer::Unbind();
    }

    void OpenGLGraphicsDevice::ClearFrameBuffer(const FrameBuffer& target, const ClearFlags flags, const glm::vec4& color, const float depth, const int stencil)
    {
        OpenGLFrameBuffer::Clear(target, flags, color, depth, stencil);
    }

    void OpenGLGraphicsDevice::ClearFrameBufferColor(const FrameBuffer& target, const std::uint32_t attachmentIndex, const glm::vec4& color)
    {
        OpenGLFrameBuffer::ClearColor(target, attachmentIndex, color);
    }

    void OpenGLGraphicsDevice::ClearFrameBufferDepth(const FrameBuffer& target, const float depth)
    {
        OpenGLFrameBuffer::ClearDepth(target, depth);
    }

    void OpenGLGraphicsDevice::ClearFrameBufferStencil(const FrameBuffer& target, const int stencil)
    {
        OpenGLFrameBuffer::ClearStencil(target, stencil);
    }

    void OpenGLGraphicsDevice::BlitFrameBuffer(const FrameBuffer& source, FrameBuffer& target, const ClearFlags mask)
    {
        OpenGLFrameBuffer::Blit(source, target, mask);
    }

    bool OpenGLGraphicsDevice::IsFrameBufferValid(const std::uint32_t framebuffer)
    {
        return OpenGLFrameBuffer::IsValid(framebuffer);
    }
}
