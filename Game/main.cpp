//
// Created by sunvy on 21/07/2026.
//

#include "GLFWWindow.h"
#ifdef SUNSET_VULKAN
#include "VulkanGraphicsDevice.h"
#elif defined(SUNSET_OPENGL)
#include "OpenGLGraphicsDevice.h"
#endif
#include "Core/Application.h"
#include "Core/GameModuleLoader.h"
#include "Render/Core/RenderCommand.h"

#include <cstdlib>
#include <exception>
#include <iostream>

namespace
{
    constexpr Sunset::WindowGraphicsAPI SelectGraphicsAPI()
    {
    #ifdef SUNSET_VULKAN
        return Sunset::WindowGraphicsAPI::Vulkan;
    #elif defined(SUNSET_OPENGL)
        return Sunset::WindowGraphicsAPI::OpenGL;
    #else
        return Sunset::WindowGraphicsAPI::None;
    #endif
    }
}

int main()
{
    try
    {
        Sunset::WindowSetting setting{"SunsetCraft"};
        setting.GraphicsAPI = SelectGraphicsAPI();

        Sunset::Application app{setting};
        app.SetWindow(std::make_unique<Sunset::GLFWWindow>(setting));
#ifdef SUNSET_VULKAN
        Sunset::RenderCommand::SetRenderAPI(std::make_unique<Sunset::VulkanGraphicsDevice>());
#elif defined(SUNSET_OPENGL)
        Sunset::RenderCommand::SetRenderAPI(std::make_unique<Sunset::OpenGLGraphicsDevice>());
#endif
        Sunset::GameModuleLoader loader;
        loader.Load(LOCAL_PATH  "SunsetCraft.so", app);
        app.Run();
        loader.Unload();
        return EXIT_SUCCESS;
    }
    catch (const std::exception& exception)
    {
        std::cerr << "[Fatal] " << exception.what() << '\n';
        return EXIT_FAILURE;
    }
}
