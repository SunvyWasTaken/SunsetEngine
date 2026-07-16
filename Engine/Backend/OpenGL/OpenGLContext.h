//
// Created by sunvy on 16/07/2026.
//

#pragma once

#include "Render/Core/GraphicsContext.h"

class GLFWwindow;

namespace Sunset
{

    class OpenGLContext : public GraphicsContext
    {
    public:
        explicit OpenGLContext(GLFWwindow* windowHandle);

        void Init() override;
        void MakeCurrent() override;
        void SwapBuffers() override;

    private:
        GLFWwindow* m_WindowHandle;
    };
} // Sunset