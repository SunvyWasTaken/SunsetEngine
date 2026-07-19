//
// Created by sunvy on 06/07/2026.
//

#include "GLFWWindow.h"
#include "Core/EditorApplication.h"

#include "Layers/EditorLayer.h"

int main()
{
    const Sunset::WindowSetting& setting{"Sunset Editor"};
    Sunset::EditorApplication app{setting};
    app.SetWindow(std::make_unique<Sunset::GLFWWindow>(setting));
    app.PushLayer<Sunset::EditorLayer>();
    app.Run();
}
