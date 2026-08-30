//
// Created by sunvy on 30/08/2026.
//

#include "OpenGLContext.h"

namespace Sunset
{
    OpenGLContext::OpenGLContext(GLFWwindow *window)
        : _window(window)
    {
    }

    OpenGLContext::~OpenGLContext()
    {
    }

    void OpenGLContext::Init()
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifndef NDEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(_window);
    }
} // Sunset