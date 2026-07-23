//
// Created by Codex on 23/07/2026.
//

#include "OpenGLGraphicsDevice.h"

#include "OpenGLShader.h"

namespace Sunset
{
    ShaderHandle OpenGLGraphicsDevice::CreateShader(const std::string_view vertexSource, const std::string_view fragmentSource)
    {
        return ShaderHandle{OpenGLShader::Create(vertexSource, fragmentSource)};
    }

    void OpenGLGraphicsDevice::DestroyShader(const ShaderHandle shader)
    {
        OpenGLShader::Destroy(shader.id);
    }

    void OpenGLGraphicsDevice::BindShader(const ShaderHandle shader)
    {
        OpenGLShader::Bind(shader.id);
    }

    void OpenGLGraphicsDevice::SetShaderFloat(const ShaderHandle shader, const std::string_view name, const float value)
    {
        OpenGLShader::SetFloat(shader.id, name, value);
    }

    void OpenGLGraphicsDevice::SetShaderInt(const ShaderHandle shader, const std::string_view name, const int value)
    {
        OpenGLShader::SetInt(shader.id, name, value);
    }

    void OpenGLGraphicsDevice::SetShaderVec2(const ShaderHandle shader, const std::string_view name, const glm::vec2& value)
    {
        OpenGLShader::SetVec2(shader.id, name, value);
    }

    void OpenGLGraphicsDevice::SetShaderVec3(const ShaderHandle shader, const std::string_view name, const glm::vec3& value)
    {
        OpenGLShader::SetVec3(shader.id, name, value);
    }

    void OpenGLGraphicsDevice::SetShaderVec4(const ShaderHandle shader, const std::string_view name, const glm::vec4& value)
    {
        OpenGLShader::SetVec4(shader.id, name, value);
    }

    void OpenGLGraphicsDevice::SetShaderMat4(const ShaderHandle shader, const std::string_view name, const glm::mat4& value)
    {
        OpenGLShader::SetMat4(shader.id, name, value);
    }
}
