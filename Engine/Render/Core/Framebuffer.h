//
// Created by sunvy on 26/08/2026.
//

#pragma once

namespace Sunset
{
    struct FramebufferSpecification
    {
        std::uint32_t Width, Height;
        std::uint32_t Samples = 1;

        bool SwapChainTarget = false;
    };

    class Framebuffer
    {
    public:
        static std::shared_ptr<Framebuffer> Create(const FramebufferSpecification& spec);
    public:
        virtual ~Framebuffer() = default;

        virtual std::uint32_t GetColorAttachmentRenderID() const = 0;

        virtual const FramebufferSpecification& GetSpecification() const = 0;

        virtual void Bind() = 0;
        virtual void UnBind() = 0;
    };
} // Sunset