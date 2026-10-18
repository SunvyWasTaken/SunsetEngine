//
// Created by sunvy on 21/07/2026.
//

#include "GLFWWindow.h"

#include "Core/Application.h"
#include "Core/GameModuleLoader.h"

int main()
{
    Sunset::GameModuleLoader loader;

    {
        Sunset::WindowSetting setting{SUNSET_GAME_NAME};
        Sunset::Application app{setting};
        app.SetWindow(std::make_unique<Sunset::GLFWWindow>(setting));
        app.InitializeWindow();
        loader.Load(SUNSET_GAME_MODULE_PATH, app);
        app.Run();
    }

    loader.Unload();
    return EXIT_SUCCESS;
}
