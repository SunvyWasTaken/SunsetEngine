//
// Created by sunvy on 20/07/2026.
//

#pragma once

#include "GameModule.h"

namespace Sunset
{
    class Application;

    class GameModuleLoader final
    {
    public:
        GameModuleLoader();

        bool Load(const std::filesystem::path& path, Application& app);

        void Unload();
    private:
        void* m_Handle;

        IGameModule* module;

        CreateGameModuleFn m_Create;
        DestroyGameModuleFn m_Destroy;
    };
}
