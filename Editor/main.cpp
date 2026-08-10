//
// Created by sunvy on 06/07/2026.
//

#include "GLFWWindow.h"
#include "OpenGLGraphicsDevice.h"
#include "Core/EditorApplication.h"
#include "Layers/EditorLayer.h"
#include "Render/Core/RenderCommand.h"

int main()
{
    const Sunset::WindowSetting setting{"Sunset Editor : " SUNSET_EDITOR_GAME_NAME};
    Sunset::EditorApplication app{setting};
    app.SetWindow(std::make_unique<Sunset::GLFWWindow>(setting));
    Sunset::RenderCommand::SetRenderAPI(std::make_unique<Sunset::OpenGLGraphicsDevice>());
    app.InitializeWindow();
    app.PushLayer<Sunset::EditorLayer>();
    app.Run();
}
