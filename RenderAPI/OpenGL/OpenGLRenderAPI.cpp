//
// Created by sunvy on 25/08/2026.
//

#include "OpenGLRenderAPI.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

    OpenGLRenderAPI::~OpenGLRenderAPI()
    {
    }

    void OpenGLRenderAPI::Init()
    {

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
} // Sunset