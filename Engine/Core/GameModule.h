//
// Created by sunvy on 20/07/2026.
//

#pragma once

#ifdef _WIN32
#define SUNSET_GAME_EXPORT __declspec(dllexport)
#else
#define SUNSET_GAME_EXPORT __attribute__((visibility("default")))
#endif

#define CREATE_MODULE(name) \
extern "C" \
{ \
    SUNSET_GAME_EXPORT Sunset::IGameModule* SunsetCreateGameModule() \
    { \
        return new name; \
    } \
 \
    SUNSET_GAME_EXPORT void SunsetDestroyGameModule(Sunset::IGameModule* module) \
    { \
        delete module; \
    } \
}

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
