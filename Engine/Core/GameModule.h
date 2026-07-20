//
// Created by sunvy on 20/07/2026.
//

#pragma once

namespace Sunset
{
    class Application;

    struct IGameModule
    {
        virtual ~IGameModule() = default;
        virtual void Load(Application &app) = 0;
        virtual void Unload() = 0;
    };

    using CreateGameModuleFn = IGameModule* (*)();
    using DestroyGameModuleFn = void (*)(IGameModule*);
}
