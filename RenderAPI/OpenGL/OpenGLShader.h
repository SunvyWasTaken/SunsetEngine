//
// Created by sunvy on 16/07/2026.
//

#pragma once

#include <cstdint>
#include <string_view>

namespace Sunset::OpenGLShader
{
    [[nodiscard]] std::uint32_t Create(std::string_view vertexSource, std::string_view fragmentSource);
    void Destroy(std::uint32_t shader);
    void Bind(std::uint32_t shader);
    void SetFloat(std::uint32_t shader, std::string_view name, float value);
    void SetInt(std::uint32_t shader, std::string_view name, int value);
    void SetVec2(std::uint32_t shader, std::string_view name, const glm::vec2& value);
    void SetVec3(std::uint32_t shader, std::string_view name, const glm::vec3& value);
    void SetVec4(std::uint32_t shader, std::string_view name, const glm::vec4& value);
    void SetMat4(std::uint32_t shader, std::string_view name, const glm::mat4& value);
}
