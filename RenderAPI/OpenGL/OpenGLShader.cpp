//
// Created by sunvy on 16/07/2026.
//

#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <string>

namespace
{
    GLint UniformLocation(const std::uint32_t shader, const std::string_view name)
    {
        const std::string uniformName(name);
        return glGetUniformLocation(shader, uniformName.c_str());
    }

    bool CheckShaderCompileStatus(const std::uint32_t shader, const std::string_view stage)
    {
        GLint success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success)
            return true;

        std::array<char, 1024> infoLog{};
        glGetShaderInfoLog(shader, static_cast<GLsizei>(infoLog.size()), nullptr, infoLog.data());
        LOG("OpenGL", error, "{} shader compilation failed: {}", stage, infoLog.data())
        return false;
    }

    bool CheckProgramLinkStatus(const std::uint32_t program)
    {
        GLint success = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (success)
            return true;

        std::array<char, 1024> infoLog{};
        glGetProgramInfoLog(program, static_cast<GLsizei>(infoLog.size()), nullptr, infoLog.data());
        LOG("OpenGL", error, "Shader program link failed: {}", infoLog.data())
        return false;
    }
}

namespace Sunset::OpenGLShader
{
    std::uint32_t Create(const std::string_view vertexSource, const std::string_view fragmentSource)
    {
        const std::string vertex(vertexSource);
        const std::string fragment(fragmentSource);
        const char* vertexSourcePtr = vertex.c_str();
        const char* fragmentSourcePtr = fragment.c_str();

        const std::uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSourcePtr, nullptr);
        glCompileShader(vertexShader);
        CheckShaderCompileStatus(vertexShader, "Vertex");

        const std::uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSourcePtr, nullptr);
        glCompileShader(fragmentShader);
        CheckShaderCompileStatus(fragmentShader, "Fragment");

        const std::uint32_t shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        CheckProgramLinkStatus(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return shaderProgram;
    }

    void Destroy(const std::uint32_t shader)
    {
        glDeleteProgram(shader);
    }

    void Bind(const std::uint32_t shader)
    {
        glUseProgram(shader);
    }

    void SetFloat(const std::uint32_t shader, const std::string_view name, const float value)
    {
        glUniform1f(UniformLocation(shader, name), value);
    }

    void SetInt(const std::uint32_t shader, const std::string_view name, const int value)
    {
        glUniform1i(UniformLocation(shader, name), value);
    }

    void SetVec2(const std::uint32_t shader, const std::string_view name, const glm::vec2& value)
    {
        glUniform2fv(UniformLocation(shader, name), 1, glm::value_ptr(value));
    }

    void SetVec3(const std::uint32_t shader, const std::string_view name, const glm::vec3& value)
    {
        glUniform3fv(UniformLocation(shader, name), 1, glm::value_ptr(value));
    }

    void SetVec4(const std::uint32_t shader, const std::string_view name, const glm::vec4& value)
    {
        glUniform4fv(UniformLocation(shader, name), 1, glm::value_ptr(value));
    }

    void SetMat4(const std::uint32_t shader, const std::string_view name, const glm::mat4& value)
    {
        glUniformMatrix4fv(UniformLocation(shader, name), 1, GL_FALSE, glm::value_ptr(value));
    }
}
