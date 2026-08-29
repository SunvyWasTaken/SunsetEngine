//
// Created by sunvy on 26/08/2026.
//

#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace
{
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

namespace Sunset
{
    OpenGLShader::OpenGLShader(const std::string_view &vertShader, const std::string_view &fragShader)
    {
        const std::string vertex(vertShader);
        const std::string fragment(fragShader);
        const char* vertexSourcePtr = vertex.c_str();
        const char* fragmentSourcePtr = fragment.c_str();

        const std::uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSourcePtr, nullptr);
        glCompileShader(vertexShader);
        if (!CheckShaderCompileStatus(vertexShader, "Vertex"))
            throw std::runtime_error("Vertex shader compilation failed");

        const std::uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSourcePtr, nullptr);
        glCompileShader(fragmentShader);
        if (!CheckShaderCompileStatus(fragmentShader, "Fragment"))
            throw std::runtime_error("Fragment shader compilation failed");

        id = glCreateProgram();
        glAttachShader(id, vertexShader);
        glAttachShader(id, fragmentShader);
        glLinkProgram(id);
        if (!CheckProgramLinkStatus(id))
            throw std::runtime_error("Program linking failed");

        LOG("OpenGL", trace, "OpenGL shader {} created succes", id)

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    OpenGLShader::~OpenGLShader()
    {
        LOG("OpenGL", trace, "OpenGL shader {} destroyed", id)
        glDeleteProgram(id);
    }

    void OpenGLShader::Bind() const
    {
        glUseProgram(id);
    }

    ShaderHandle OpenGLShader::Get() const
    {
        return id;
    }

    void OpenGLShader::SetFloat(const std::string_view &name, float value) const
    {
        glUniform1f(glGetUniformLocation(id, name.data()), value);
    }

    void OpenGLShader::SetInt(const std::string_view &name, int value) const
    {
        glUniform1i(glGetUniformLocation(id, name.data()), value);
    }

    void OpenGLShader::SetVec2(const std::string_view &name, const glm::vec2 &value) const
    {
        glUniform2fv(glGetUniformLocation(id, name.data()), 1, glm::value_ptr(value));
    }

    void OpenGLShader::SetVec3(const std::string_view &name, const glm::vec3 &value) const
    {
        glUniform3fv(glGetUniformLocation(id, name.data()), 1, glm::value_ptr(value));
    }

    void OpenGLShader::SetVec4(const std::string_view &name, const glm::vec4 &value) const
    {
        glUniform4fv(glGetUniformLocation(id, name.data()), 1, glm::value_ptr(value));
    }

    void OpenGLShader::SetMat4(const std::string_view &name, const glm::mat4 &value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(id, name.data()), 1, GL_FALSE, glm::value_ptr(value));
    }
} // Sunset