//SUNSET_USE_OPENGL
// Created by sunvy on 30/08/2026.
//

#pragma once

#include <GLFW/glfw3.h>

#include "Core/GraphicContext.h"

namespace Sunset
{
    class OpenGLContext final : public GraphicsContext
    {
    public:
        explicit OpenGLContext(GLFWwindow *window);

        ~OpenGLContext() override;

        void Init() override;

        void SwapBuffers() override;
    private:
        GLFWwindow* _window;
    };
} // Sunset