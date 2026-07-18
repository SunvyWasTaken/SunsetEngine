//
// Created by sunvy on 06/07/2026.
//

#include "SunsetEngine.h"
#include "Core/EditorApplication.h"
#include "Core/EditorApplicationEntrypoint.h"

#include "Layers/EditorLayer.h"

int main()
{
    Sunset::EditorApplication app;
    app.PushLayer<Sunset::EditorLayer>();
    Sunset::ConfigureEditorApplication(app);
    app.Run();
}
