//
// Created by sunvy on 26/08/2026.
//

#pragma once
#include "Render/Core/Framebuffer.h"

namespace Sunset
{
    class OpenGLFramebuffer final : public Framebuffer
    {
    public:
        explicit OpenGLFramebuffer(const FramebufferSpecification& specification);
        ~OpenGLFramebuffer() override;

        std::uint32_t GetColorAttachmentRenderID() const override;

        const FramebufferSpecification & GetSpecification() const override { return spec; }

        void Invalidate();

        void Bind() override;

        void UnBind() override;

    private:

        std::uint32_t m_RenderID;
        std::uint32_t m_ColorAttachment, m_DepthAttachment;
        FramebufferSpecification spec;
    };
} // Sunset