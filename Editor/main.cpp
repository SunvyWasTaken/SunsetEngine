//
// Created by sunvy on 06/07/2026.
//

#include "SunsetCraftModule.h"
#include "SunsetEngine.h"
#include "Core/EditorApplication.h"

#include "Layers/EditorLayer.h"

int main()
{
    Sunset::EditorApplication app;
    app.PushLayer<Sunset::EditorLayer>();
    GameModule::InitGame(app);
    app.Run();
}
