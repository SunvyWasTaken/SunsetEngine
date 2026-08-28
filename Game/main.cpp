//
// Created by sunvy on 21/07/2026.
//

#include "GLFWWindow.h"
#ifdef SUNSET_VULKAN
#include "VulkanGraphicsDevice.h"
#elif defined(SUNSET_OPENGL)
#include "OpenGLRenderAPI.h"
#endif

#include "Core/Application.h"
#include "Core/GameModuleLoader.h"
#include "Render/Core/RenderCommand.h"

int main()
{
    Sunset::WindowSetting setting{SUNSET_GAME_NAME};

    Sunset::Application app{setting};
    app.SetWindow(std::make_unique<Sunset::GLFWWindow>(setting));
#ifdef SUNSET_VULKAN
    Sunset::RenderCommand::SetRenderAPI(std::make_unique<Sunset::VulkanGraphicsDevice>());
#elif defined(SUNSET_OPENGL)
    Sunset::RenderCommand::SetRenderAPI(std::make_unique<Sunset::OpenGLRenderAPI>());
#endif
    app.InitializeWindow();
    Sunset::GameModuleLoader loader;
    loader.Load(SUNSET_GAME_MODULE_PATH, app);
    app.Run();
    loader.Unload();
    return EXIT_SUCCESS;
}
