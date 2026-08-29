//
// Created by sunvy on 15/12/2025.
//

#pragma once

namespace Sunset
{
    struct ShaderHandle;

    class Shader
    {
    public:
        static std::unique_ptr<Shader> CreateShader(const std::string_view& vertShader, const std::string_view& fragShader);
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;

        [[nodiscard]] virtual ShaderHandle Get() const = 0;

        virtual void SetFloat(const std::string_view& name, float value) const = 0;
        virtual void SetInt(const std::string_view& name, int value) const = 0;
        virtual void SetVec2(const std::string_view& name, const glm::vec2& value) const = 0;
        virtual void SetVec3(const std::string_view& name, const glm::vec3& value) const = 0;
        virtual void SetVec4(const std::string_view& name, const glm::vec4& value) const = 0;
        virtual void SetMat4(const std::string_view& name, const glm::mat4& value) const = 0;
    };
} // Sunset