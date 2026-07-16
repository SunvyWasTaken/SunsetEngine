//
// Created by sunvy on 16/07/2026.
//

#include "Window.h"

#include "GLFW/GLFWWindow.h"

namespace Sunset
{
    std::unique_ptr<Window> Window::CreateWindow(const WindowSetting &setting)
    {
        return std::make_unique<GLFWWindow>(setting);
    }
}
