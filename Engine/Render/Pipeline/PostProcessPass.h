//
// Created by Codex on 23/07/2026.
//

#pragma once

#include "RenderPass.h"

#include "Render/FrameBuffer.h"
#include "Render/Core/RenderHandle.h"

namespace Sunset
{
    struct PostProcessPassSpecification
    {
        int width = 1;
        int height = 1;
        bool hdr = true;
    };

    class PostProcessPass final : public RenderPass
    {
    public:
        explicit PostProcessPass(const PostProcessPassSpecification& specification);
        ~PostProcessPass() override;

        void Begin(const glm::vec4& clearColor = glm::vec4(0.0f));
        void End();
        void Resize(int width, int height);

        [[nodiscard]] TextureHandle GetColorTexture() const;
        [[nodiscard]] TextureHandle GetDepthTexture() const;
        [[nodiscard]] const FrameBuffer& GetFrameBuffer() const;
        [[nodiscard]] FrameBuffer& GetFrameBuffer();

    private:
        static FrameBufferSpecification CreateFrameBufferSpecification(const PostProcessPassSpecification& specification);

        PostProcessPassSpecification m_Specification;
        FrameBuffer m_FrameBuffer;
    };
}
