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
        using Application::Application;
        ~EditorApplication() override;
        void OnDestroy() override;
        void BeginFrame() override;
        void EndFrame() override;
    protected:
        void OnWindowReady() override;

    private:
        GameModuleLoader loader;
    };
} // Sunset
