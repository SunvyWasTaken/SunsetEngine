//
// Created by sunvy on 26/08/2026.
//

#pragma once

namespace Sunset
{
    struct RenderTargetSpecification
    {
        std::uint32_t Width, Height;
        std::uint32_t Samples = 1;

        bool SwapChainTarget = false;
    };

    class RenderTarget
    {
    public:
        static std::shared_ptr<RenderTarget> Create(const RenderTargetSpecification& spec);
    public:
        virtual ~RenderTarget() = default;

        virtual std::uint32_t GetColorAttachmentRenderID() const = 0;

        virtual const RenderTargetSpecification& GetSpecification() const = 0;

        virtual void Bind() = 0;
        virtual void UnBind() = 0;
    };
} // Sunset