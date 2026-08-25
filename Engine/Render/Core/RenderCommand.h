//
// Created by sunvy on 25/08/2026.
//

#pragma once
#include <memory>

#include "Render/Resources/Texture.h"

namespace Sunset
{
    struct ShaderHandle;

    struct RenderCommand
    {
        static void SetRenderAPI(std::unique_ptr<struct RenderAPI> renderAPI);
        static bool HasRenderAPI();

        static void Shutdown();

        static void BeginFrame();
        static void EndFrame();

        /*--------------------------------------*/
        /* Shader                               */
        /*--------------------------------------*/
        static ShaderHandle CreateShader(const std::string& vertShader, const std::string& fragShader);
        static void DestroyShader(ShaderHandle& shaderID);

        static void BindShader(const ShaderHandle& shaderID);

        static void SetShaderFloat(const ShaderHandle& shaderID, const std::string_view& name, float value);
        static void SetShaderInt(const ShaderHandle& shaderID, const std::string_view& name, int value);
        static void SetShaderVec2(const ShaderHandle& shaderID, const std::string_view& name, const glm::vec2& value);
        static void SetShaderVec3(const ShaderHandle& shaderID, const std::string_view& name, const glm::vec3& value);
        static void SetShaderVec4(const ShaderHandle& shaderID, const std::string_view& name, const glm::vec4& value);
        static void SetShaderMat4(const ShaderHandle& shaderID, const std::string_view& name, const glm::mat4& value);

        /*--------------------------------------*/
        /* Texture                              */
        /*--------------------------------------*/
        static TextureHandle CreateTexture(const void* data = nullptr);
        static void DestroyTexture(TextureHandle& textureID);
        static void BindTexture(const TextureHandle& texture, std::uint32_t slot);
    };
} // Sunset