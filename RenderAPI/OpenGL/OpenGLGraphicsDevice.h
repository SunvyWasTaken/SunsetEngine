//
// Created by sunvy on 16/07/2026.
//

#pragma once

#include "Render/Core/RenderAPI.h"
#include "Render/Core/RenderType.h"

namespace Sunset
{
    class Material;

    class OpenGLGraphicsDevice final : public RenderAPI
    {
    public:
        ~OpenGLGraphicsDevice() override;

        void Init() override;
        void BeginFrame() override;
        void EndFrame() override;

        void BeginTarget(FrameBuffer& target, const glm::vec4& clearColor) override;
        void EndTarget() override;
        void Flush() override;

        void Submit(const Drawable& drawable, const glm::mat4& model) override;
        void UseCamera(const Camera& camera) override;
        void ShowCursor(bool show) override;

        void SetViewport(const glm::ivec2& size) override;
        void Clear(const glm::vec4& color) override;

    private:
        struct FrameData
        {
            glm::vec3 position = glm::vec3(0.0f);
            glm::mat4 view = glm::mat4(1.0f);
            glm::mat4 projection = glm::mat4(1.0f);
        };

        struct DrawCommand
        {
            uint32_t vao = 0;
            uint32_t indexCount = 0;
            std::shared_ptr<Material> material;
            glm::mat4 model = glm::mat4(1.0f);
            RenderState state;
        };

        void ResetFrameState() const;
        void ApplyState(const RenderState& state) const;
        void SortDrawCommands();
        void FlushDrawCommands();

        FrameData m_FrameData;
        std::vector<DrawCommand> m_DrawCommands;
    };
} // Sunset