//
// Created by sunvy on 25/08/2026.
//

#pragma once

#include "Render/Core/RenderAPI.h"

namespace Sunset
{
    struct OpenGLRenderAPI : public RenderAPI
    {
        OpenGLRenderAPI();
        ~OpenGLRenderAPI() override;

        void Init() override;
        void Clear(const glm::vec4& color) override;
        void BeginFrame() override;
        void EndFrame() override;

        /*--------------------------------------*/
        /* Shader                               */
        /*--------------------------------------*/
        ShaderHandle CreateShader(const std::string& vertShader, const std::string& fragShader) override;
        void DestroyShader(const ShaderHandle& shaderID) override;
        void BindShader(const ShaderHandle& shaderID) override;
        void SetShaderFloat(const ShaderHandle& shaderID, const std::string_view& name, float value) override;
        void SetShaderInt(const ShaderHandle& shaderID, const std::string_view& name, int value) override;
        void SetShaderVec2(const ShaderHandle& shaderID, const std::string_view& name, const glm::vec2& value) override;
        void SetShaderVec3(const ShaderHandle& shaderID, const std::string_view& name, const glm::vec3& value) override;
        void SetShaderVec4(const ShaderHandle& shaderID, const std::string_view& name, const glm::vec4& value) override;
        void SetShaderMat4(const ShaderHandle& shaderID, const std::string_view& name, const glm::mat4& value) override;

        /*--------------------------------------*/
        /* Texture                              */
        /*--------------------------------------*/
        TextureHandle CreateTexture(const void* data = nullptr);
        void DestroyTexture(const TextureHandle& textureID) override;
        void BindTexture(const TextureHandle& textureID, std::uint32_t slot);

    private:

        struct OpenGLData;
        std::unique_ptr<OpenGLData> data;
    };
} // Sunset