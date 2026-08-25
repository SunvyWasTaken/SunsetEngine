//
// Created by sunvy on 25/08/2026.
//

#include "OpenGLRenderAPI.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGLDrawQueue.h"

namespace
{

}

namespace Sunset
{
    struct OpenGLRenderAPI::OpenGLData
    {
        GLFWwindow* window;
        OpenGLDrawQueue m_DrawQueue;
    };

    OpenGLRenderAPI::OpenGLRenderAPI()
    {
        INITLOG("OpenGL")
    }

    OpenGLRenderAPI::~OpenGLRenderAPI()
    {
    }

    void OpenGLRenderAPI::Init()
    {
        LOG("OpenGL", info, "OpenGLRenderAPI init")
        const int loader = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

        if (loader == GL_FALSE)
            throw std::runtime_error("Failed to initialize GLAD");
    }

    void OpenGLRenderAPI::Clear(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRenderAPI::BeginFrame()
    {

    }

    void OpenGLRenderAPI::EndFrame()
    {

    }
}

/*--------------------------------------*/
/* Shader                               */
/*--------------------------------------*/

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
    ShaderHandle OpenGLRenderAPI::CreateShader(const std::string &vertShader, const std::string &fragShader)
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

        const std::uint32_t shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        if (!CheckProgramLinkStatus(shaderProgram))
            throw std::runtime_error("Program linking failed");

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return ShaderHandle{shaderProgram};
    }

    void OpenGLRenderAPI::DestroyShader(const ShaderHandle &shaderID)
    {
        glDeleteProgram(shaderID());
    }

    void OpenGLRenderAPI::BindShader(const ShaderHandle &shaderID)
    {
        glUseProgram(shaderID());
    }

    void OpenGLRenderAPI::SetShaderFloat(const ShaderHandle &shaderID, const std::string_view &name, float value)
    {
        glUniform1f(glGetUniformLocation(shaderID(), name.data()), value);
    }

    void OpenGLRenderAPI::SetShaderInt(const ShaderHandle &shaderID, const std::string_view &name, int value)
    {
        glUniform1i(glGetUniformLocation(shaderID(), name.data()), value);
    }

    void OpenGLRenderAPI::SetShaderVec2(const ShaderHandle &shaderID, const std::string_view &name, const glm::vec2 &value)
    {
        glUniform2fv(glGetUniformLocation(shaderID(), name.data()), 1, glm::value_ptr(value));
    }

    void OpenGLRenderAPI::SetShaderVec3(const ShaderHandle &shaderID, const std::string_view &name, const glm::vec3 &value)
    {
        glUniform3fv(glGetUniformLocation(shaderID(), name.data()), 1, glm::value_ptr(value));
    }

    void OpenGLRenderAPI::SetShaderVec4(const ShaderHandle &shaderID, const std::string_view &name, const glm::vec4 &value)
    {
        glUniform4fv(glGetUniformLocation(shaderID(), name.data()), 1, glm::value_ptr(value));
    }

    void OpenGLRenderAPI::SetShaderMat4(const ShaderHandle &shaderID, const std::string_view &name, const glm::mat4 &value)
    {
        glUniformMatrix4fv(glGetUniformLocation(shaderID(), name.data()), 1, GL_FALSE, glm::value_ptr(value));
    }

    TextureHandle OpenGLRenderAPI::CreateTexture(const void *data)
    {
        return TextureHandle{};
    }

    void OpenGLRenderAPI::DestroyTexture(const TextureHandle &textureID)
    {
    }

    void OpenGLRenderAPI::BindTexture(const TextureHandle &textureID, std::uint32_t slot)
    {
    }
} // Sunset