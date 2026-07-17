//
// Created by sunvy on 16/07/2026.
//

#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "SRmGUI.h"
#include "SRmGUI_Opengl.h"

namespace Sunset
{
    OpenGLContext::OpenGLContext(GLFWwindow *windowHandle)
        : GraphicsContext()
        , m_WindowHandle(windowHandle)
    {
        if (m_WindowHandle == nullptr)
            throw std::runtime_error("OpenGLContext received a null windowHandle");
    }

    OpenGLContext::~OpenGLContext()
    {
        SRmGUI::Opengl_Shutdown();
    }

    void OpenGLContext::Init()
    {
        MakeCurrent();

        const int loader = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

        SRmGUI::Opengl_Init();

        if (loader == GL_FALSE)
            throw std::runtime_error("Failed to initialize GLAD");
    }

    void OpenGLContext::MakeCurrent()
    {
        glfwMakeContextCurrent(m_WindowHandle);
    }

    void OpenGLContext::SwapBuffers()
    {
        SRmGUI::Opengl_DrawData(SRmGUI::GetDrawData());
        glfwSwapBuffers(m_WindowHandle);
    }
} // Sunset