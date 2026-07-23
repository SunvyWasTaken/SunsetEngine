//
// Created by Codex on 23/07/2026.
//

#include "ShadowPass.h"

#include "Render/Core/RenderCommand.h"
#include "Render/Core/RenderType.h"
#include "Render/Core/Shader.h"
#include "Render/FrameBufferPresets.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>

namespace Sunset
{
    ShadowPass::ShadowPass(const ShadowPassSpecification& specification)
        : RenderPass("ShadowPass")
        , m_Specification(specification)
        , m_FrameBuffer(FrameBufferPresets::ShadowMap(specification.size))
    {
        m_Shader = std::make_unique<Shader>(m_Specification.vertexShaderPath, m_Specification.fragmentShaderPath);
        m_IsValid = m_FrameBuffer.IsValid();
    }

    ShadowPass::~ShadowPass() = default;

    void ShadowPass::Render(const glm::vec3& center, const glm::vec3& lightDirection, const DrawCallback& drawCallback)
    {
        if (!m_IsValid || lightDirection.y <= -0.08f || !drawCallback)
            return;

        m_LightSpaceMatrix = BuildLightSpaceMatrix(center, lightDirection);

        RenderCommand::BeginTarget(m_FrameBuffer);
        RenderCommand::SetCullMode(CullMode::Front);

        m_Shader->Use();
        m_Shader->SetMat4("u_LightSpaceMatrix", m_LightSpaceMatrix);
        drawCallback(*m_Shader);

        RenderCommand::SetCullMode(CullMode::Back);
        RenderCommand::EndTarget();
    }

    void ShadowPass::BindForRead(const int textureUnit) const
    {
        RenderCommand::BindTexture(m_FrameBuffer.GetDepthAttachment(), static_cast<std::uint32_t>(textureUnit));
    }

    ShadowRenderData ShadowPass::GetRenderData(const int textureUnit) const
    {
        return {
            .enabled = m_IsValid,
            .depthTexture = m_FrameBuffer.GetDepthAttachment(),
            .textureUnit = textureUnit,
            .lightSpaceMatrix = m_LightSpaceMatrix
        };
    }

    const glm::mat4& ShadowPass::GetLightSpaceMatrix() const
    {
        return m_LightSpaceMatrix;
    }

    std::uint32_t ShadowPass::GetDepthTexture() const
    {
        return m_FrameBuffer.GetDepthAttachment();
    }

    bool ShadowPass::IsValid() const
    {
        return m_IsValid;
    }

    glm::mat4 ShadowPass::BuildLightSpaceMatrix(const glm::vec3& center, const glm::vec3& lightDirection) const
    {
        const glm::vec3 lightDir = glm::normalize(lightDirection);
        const glm::vec3 lightPosition = center + lightDir * m_Specification.lightHeight;
        const glm::vec3 up = std::abs(lightDir.y) > 0.95f ? glm::vec3{0.0f, 0.0f, 1.0f} : glm::vec3{0.0f, 1.0f, 0.0f};

        const glm::mat4 lightView = glm::lookAt(lightPosition, center, up);
        const glm::mat4 lightProjection = glm::ortho(
            -m_Specification.distance,
            m_Specification.distance,
            -m_Specification.distance,
            m_Specification.distance,
            1.0f,
            m_Specification.lightHeight * 2.4f);

        return lightProjection * lightView;
    }
}
