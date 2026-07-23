//
// Created by Codex on 23/07/2026.
//

#include "PostProcessPass.h"

#include "Render/Core/RenderCommand.h"

namespace Sunset
{
    PostProcessPass::PostProcessPass(const PostProcessPassSpecification& specification)
        : RenderPass("PostProcessPass")
        , m_Specification(specification)
        , m_FrameBuffer(CreateFrameBufferSpecification(specification))
    {
    }

    PostProcessPass::~PostProcessPass() = default;

    void PostProcessPass::Begin(const glm::vec4& clearColor)
    {
        RenderCommand::BeginTarget(m_FrameBuffer, clearColor);
    }

    void PostProcessPass::End()
    {
        RenderCommand::EndTarget();
    }

    void PostProcessPass::Resize(const int width, const int height)
    {
        m_Specification.width = width;
        m_Specification.height = height;
        m_FrameBuffer.Resize(width, height);
    }

    std::uint32_t PostProcessPass::GetColorTexture() const
    {
        return m_FrameBuffer.GetColorAttachment();
    }

    std::uint32_t PostProcessPass::GetDepthTexture() const
    {
        return m_FrameBuffer.GetDepthAttachment();
    }

    const FrameBuffer& PostProcessPass::GetFrameBuffer() const
    {
        return m_FrameBuffer;
    }

    FrameBuffer& PostProcessPass::GetFrameBuffer()
    {
        return m_FrameBuffer;
    }

    FrameBufferSpecification PostProcessPass::CreateFrameBufferSpecification(const PostProcessPassSpecification& specification)
    {
        return {
            .width = specification.width,
            .height = specification.height,
            .samples = 1,
            .attachments = {
                {
                    specification.hdr ? FrameBufferTextureFormat::RGBA16F : FrameBufferTextureFormat::RGBA8,
                    TextureFilter::Linear,
                    TextureWrap::ClampToEdge
                },
                { FrameBufferTextureFormat::Depth24Stencil8 }
            }
        };
    }
}
