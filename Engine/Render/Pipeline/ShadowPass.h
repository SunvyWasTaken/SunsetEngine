//
// Created by Codex on 23/07/2026.
//

#pragma once

#include "RenderPass.h"

#include "Render/FrameBuffer.h"
#include "Render/Core/RenderHandle.h"

#include <cstdint>
#include <functional>
#include <memory>

namespace Sunset
{
    class Shader;

    struct ShadowPassSpecification
    {
        int size = 2048;
        float distance = 128.0f;
        float lightHeight = 180.0f;
        std::string_view vertexShaderPath;
        std::string_view fragmentShaderPath;
    };

    struct ShadowRenderData
    {
        bool enabled = false;
        TextureHandle depthTexture;
        int textureUnit = 1;
        glm::mat4 lightSpaceMatrix{1.0f};
    };

    class ShadowPass final : public RenderPass
    {
    public:
        using DrawCallback = std::function<void(const Shader&)>;

        explicit ShadowPass(const ShadowPassSpecification& specification);
        ~ShadowPass() override;

        void Render(const glm::vec3& center, const glm::vec3& lightDirection, const DrawCallback& drawCallback);
        void BindForRead(int textureUnit) const;

        [[nodiscard]] ShadowRenderData GetRenderData(int textureUnit = 1) const;
        [[nodiscard]] const glm::mat4& GetLightSpaceMatrix() const;
        [[nodiscard]] TextureHandle GetDepthTexture() const;
        [[nodiscard]] bool IsValid() const;

    private:
        [[nodiscard]] glm::mat4 BuildLightSpaceMatrix(const glm::vec3& center, const glm::vec3& lightDirection) const;

        ShadowPassSpecification m_Specification;
        FrameBuffer m_FrameBuffer;
        std::unique_ptr<Shader> m_Shader;
        glm::mat4 m_LightSpaceMatrix{1.0f};
        bool m_IsValid = false;
    };
}
