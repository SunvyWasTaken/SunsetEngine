//
// Created by sunvy on 06/07/2026.
//

#include "GLFWWindow.h"
#include "OpenGLRenderAPI.h"
#include "Core/EditorApplication.h"
#include "Layers/EditorLayer.h"

int main()
{
    const Sunset::WindowSetting setting{"Sunset Editor : " SUNSET_EDITOR_GAME_NAME};
    Sunset::EditorApplication app{setting};
    app.SetWindow(std::make_unique<Sunset::GLFWWindow>(setting));
    app.SetRenderAPI(std::make_unique<Sunset::OpenGLRenderAPI>());
    app.InitializeWindow();
    app.PushLayer<Sunset::EditorLayer>();
    app.Run();
}
