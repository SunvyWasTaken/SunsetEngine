//
// Created by sunvy on 06/07/2026.
//

#include "SunsetEngine.h"
#include "Core/EditorApplication.h"

#include "Layers/EditorLayer.h"

int main()
{
    Sunset::EditorApplication app;
    app.PushLayer<Sunset::EditorLayer>();
    app.Run();
}
