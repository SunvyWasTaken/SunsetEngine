//
// Created by sunvy on 21/07/2026.
//

#include "GLFWWindow.h"
#include "OpenGLGraphicsDevice.h"
#include "Core/Application.h"
#include "Core/GameModuleLoader.h"
#include "Render/Core/RenderCommand.h"

int main()
{
    const Sunset::WindowSetting setting{"SunsetCraft"};
    Sunset::Application app{setting};
    app.SetWindow(std::make_unique<Sunset::GLFWWindow>(setting));
    Sunset::RenderCommand::SetRenderAPI(std::make_unique<Sunset::OpenGLGraphicsDevice>());
    Sunset::GameModuleLoader loader;
    loader.Load(LOCAL_PATH  "SunsetCraft.so", app);
    app.Run();
    loader.Unload();
};
