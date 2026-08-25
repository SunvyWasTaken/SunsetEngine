//
// Created by sunvy on 24/08/2026.
//

#pragma once
#include "RenderHandle.h"

namespace Sunset
{
    struct ShaderHandle;

    struct RenderAPI
    {
        virtual ~RenderAPI() = default;

        virtual void Init() = 0;
        virtual void Clear(const glm::vec4& color) = 0;
        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;

        /*--------------------------------------*/
        /* Shader                               */
        /*--------------------------------------*/
        virtual ShaderHandle CreateShader(const std::string& vertShader, const std::string& fragShader) = 0;
        virtual void DestroyShader(const ShaderHandle& shaderID) = 0;
        virtual void BindShader(const ShaderHandle& shaderID) = 0;
        virtual void SetShaderFloat(const ShaderHandle& shaderID, const std::string_view& name, float value) = 0;
        virtual void SetShaderInt(const ShaderHandle& shaderID, const std::string_view& name, int value) = 0;
        virtual void SetShaderVec2(const ShaderHandle& shaderID, const std::string_view& name, const glm::vec2& value) = 0;
        virtual void SetShaderVec3(const ShaderHandle& shaderID, const std::string_view& name, const glm::vec3& value) = 0;
        virtual void SetShaderVec4(const ShaderHandle& shaderID, const std::string_view& name, const glm::vec4& value) = 0;
        virtual void SetShaderMat4(const ShaderHandle& shaderID, const std::string_view& name, const glm::mat4& value) = 0;

        /*--------------------------------------*/
        /* Texture                              */
        /*--------------------------------------*/
        virtual TextureHandle CreateTexture(const void* data = nullptr) = 0;
        virtual void DestroyTexture(const TextureHandle& textureID) = 0;
        virtual void BindTexture(const TextureHandle& textureID, std::uint32_t slot) = 0;
    };
} // Sunset