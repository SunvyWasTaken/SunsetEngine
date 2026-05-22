//
// Created by sunvy on 15/12/2025.
//

#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>

namespace
{
    std::string OpenFile(const std::string_view& filename)
    {
        std::ifstream tmpFile;
        tmpFile.open(filename.data());
        std::stringstream tmpFileStream;
        // read file's buffer contents into streams
        tmpFileStream << tmpFile.rdbuf();
        // close file handlers
        tmpFile.close();
        // convert stream into string
        return tmpFileStream.str();
    }

    void CreateShader(const char * vertexShaderSource, const char * fragmentShaderSource, unsigned int& shaderProgram)
    {
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        int  success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);


        if(!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);


        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
}

namespace SunsetEngine
{
    Shader::Shader(const std::string_view& vertPath, const std::string_view& fragPath)
        : id(0)
    {
        const std::string vertShader = OpenFile(vertPath);
        const std::string fragShader = OpenFile(fragPath);
        CreateShader(vertShader.c_str(), fragShader.c_str(), id);
        LOG("Engine", trace, "Shader {} create succefully", id)
    }

    Shader::~Shader()
    {
        LOG("Engine", trace, "Shader {} delete", id)
        glDeleteProgram(id);
    }

    std::uint32_t Shader::GetId() const
    {
        return id;
    }

    void Shader::Use() const
    {
        glUseProgram(id);
    }

    void Shader::SetFloat(const std::string_view& name, float value) const
    {
        glUniform1f(glGetUniformLocation(id, name.data()), value);
    }

    void Shader::SetInt(const std::string_view& name, const int value) const
    {
        glUniform1i(glGetUniformLocation(id, name.data()), value);
    }

    void Shader::SetVec2(const std::string_view& name, const glm::vec2& vec) const
    {
        glUniform2fv(glGetUniformLocation(id, name.data()), 1, glm::value_ptr(vec));
    }

    void Shader::SetVec3(const std::string_view& name, const glm::vec3& value) const
    {
        glUniform3f(glGetUniformLocation(id, name.data()), value.x, value.y, value.z);
    }

    void Shader::SetVec4(const std::string_view& name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(id, name.data()), 1, glm::value_ptr(value));
    }

    void Shader::SetMat4(const std::string_view& name, const glm::mat4& value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(id, name.data()), 1, GL_FALSE, &value[0][0]);
    }
}
