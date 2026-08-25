//
// Created by sunvy on 15/12/2025.
//

#include "Shader.h"

#include "Render/Core/RenderCommand.h"
#include "Utility/UtilityFunction.h"

namespace
{
    std::string OpenFile(const std::string_view& filename)
    {
        std::ifstream file(filename.data());
        std::stringstream stream;
        stream << file.rdbuf();
        return stream.str();
    }
}

namespace Sunset
{
    Shader::Shader(const std::string_view &vertPath, const std::string_view &fragPath)
    {
        std::string vertShader;
        std::string fragShader;

        if (UtilityFunction::DoesFileExist(vertPath))
            vertShader = OpenFile(vertPath);
        if (UtilityFunction::DoesFileExist(fragPath))
            fragShader = OpenFile(fragPath);

        id = RenderCommand::CreateShader(vertShader, fragShader);

        LOG("Engine", trace, "Shader {} Created", id())
    }

    Shader::~Shader()
    {
        LOG("Engine", trace, "Shader {} deleted", id())
        RenderCommand::DestroyShader(id);
    }

    ShaderHandle Shader::GetID() const
    {
        return id;
    }

    void Shader::Use() const
    {
        RenderCommand::BindShader(id);
    }

    void Shader::SetFloat(const std::string_view &name, float value) const
    {
        RenderCommand::SetShaderFloat(id, name, value);
    }

    void Shader::SetInt(const std::string_view &name, int value) const
    {
        RenderCommand::SetShaderInt(id, name, value);
    }

    void Shader::SetVec2(const std::string_view &name, const glm::vec2 &value) const
    {
        RenderCommand::SetShaderVec2(id, name, value);
    }

    void Shader::SetVec3(const std::string_view &name, const glm::vec3 &value) const
    {
        RenderCommand::SetShaderVec3(id, name, value);
    }

    void Shader::SetVec4(const std::string_view &name, const glm::vec4 &value) const
    {
        RenderCommand::SetShaderVec4(id, name, value);
    }

    void Shader::SetMat4(const std::string_view &name, const glm::mat4 &value) const
    {
        RenderCommand::SetShaderMat4(id, name, value);
    }
} // Sunset