//
// Created by Codex on 23/07/2026.
//

#include "OpenGLGraphicsDevice.h"

#include "OpenGLShader.h"

namespace Sunset
{
    std::uint32_t OpenGLGraphicsDevice::CreateShader(const std::string_view vertexSource, const std::string_view fragmentSource)
    {
        return OpenGLShader::Create(vertexSource, fragmentSource);
    }

    void OpenGLGraphicsDevice::DestroyShader(const std::uint32_t shader)
    {
        OpenGLShader::Destroy(shader);
    }

    void OpenGLGraphicsDevice::BindShader(const std::uint32_t shader)
    {
        OpenGLShader::Bind(shader);
    }

    void OpenGLGraphicsDevice::SetShaderFloat(const std::uint32_t shader, const std::string_view name, const float value)
    {
        OpenGLShader::SetFloat(shader, name, value);
    }

    void OpenGLGraphicsDevice::SetShaderInt(const std::uint32_t shader, const std::string_view name, const int value)
    {
        OpenGLShader::SetInt(shader, name, value);
    }

    void OpenGLGraphicsDevice::SetShaderVec2(const std::uint32_t shader, const std::string_view name, const glm::vec2& value)
    {
        OpenGLShader::SetVec2(shader, name, value);
    }

    void OpenGLGraphicsDevice::SetShaderVec3(const std::uint32_t shader, const std::string_view name, const glm::vec3& value)
    {
        OpenGLShader::SetVec3(shader, name, value);
    }

    void OpenGLGraphicsDevice::SetShaderVec4(const std::uint32_t shader, const std::string_view name, const glm::vec4& value)
    {
        OpenGLShader::SetVec4(shader, name, value);
    }

    void OpenGLGraphicsDevice::SetShaderMat4(const std::uint32_t shader, const std::string_view name, const glm::mat4& value)
    {
        OpenGLShader::SetMat4(shader, name, value);
    }
}
