//
// Created by sunvy on 26/08/2026.
//

#pragma once

#include "Render/Resources/Shader.h"

namespace Sunset
{
    class OpenGLShader final : public Shader
    {
    public:
        OpenGLShader(const std::string_view& vertShader, const std::string_view& fragShader);

        ~OpenGLShader() override;

         void Bind() const override;

         void SetFloat(const std::string_view& name, float value) const override;
         void SetInt(const std::string_view& name, int value) const override;
         void SetVec2(const std::string_view& name, const glm::vec2& value) const override;
         void SetVec3(const std::string_view& name, const glm::vec3& value) const override;
         void SetVec4(const std::string_view& name, const glm::vec4& value) const override;
         void SetMat4(const std::string_view& name, const glm::mat4& value) const override;

    private:
        std::uint32_t id = 0;
    };
} // Sunset