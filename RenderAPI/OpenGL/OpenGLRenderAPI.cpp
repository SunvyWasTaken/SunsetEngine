//
// Created by sunvy on 25/08/2026.
//

#include "OpenGLRenderAPI.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "OpenGLDrawQueue.h"
#include "Resources/OpenGLShader.h"

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

namespace Sunset
{
    std::unique_ptr<Shader> OpenGLRenderAPI::CreateShader(const std::string &vertShader, const std::string &fragShader)
    {
        return std::make_unique<OpenGLShader>(vertShader, fragShader);
    }

    TextureHandle OpenGLRenderAPI::CreateTexture(const void *data)
    {
        return TextureHandle{};
    }
} // Sunset