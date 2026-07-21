 //
// Created by sunvy on 06/07/2026.
//

#pragma once

#include "Core/Application.h"
#include "Core/GameModuleLoader.h"

 namespace Sunset
{
    class EditorApplication : public Application
    {
    public:
        EditorApplication(const WindowSetting& setting = WindowSetting{});
        void OnDestroy() override;
        void BeginFrame() override;
        void EndFrame() override;
    protected:
        void OnWindowReady() override;

    private:
        GameModuleLoader loader;
    };
} // Sunset