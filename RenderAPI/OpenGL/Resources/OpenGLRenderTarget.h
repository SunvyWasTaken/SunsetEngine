//
// Created by sunvy on 26/08/2026.
//

#pragma once
#include "Render/Resources/RenderTarget.h"

namespace Sunset
{
    class OpenGLRenderTarget final : public RenderTarget
    {
    public:
        explicit OpenGLRenderTarget(const RenderTargetSpecification& specification);
        ~OpenGLRenderTarget() override;

        std::uint32_t GetColorAttachmentRenderID() const override;

        const RenderTargetSpecification & GetSpecification() const override { return spec; }

        void Invalidate();

        void Bind() override;

        void UnBind() override;

    private:

        std::uint32_t m_RenderID;
        std::uint32_t m_ColorAttachment, m_DepthAttachment;
        RenderTargetSpecification spec;
    };
} // Sunset