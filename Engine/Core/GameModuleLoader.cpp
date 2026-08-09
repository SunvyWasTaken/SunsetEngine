//
// Created by sunvy on 20/07/2026.
//

#include "GameModuleLoader.h"

#if _WIN32
#include <window.h>
#define LOADDL(...) LoadLibraryW(__VA_ARGS__)
#define GETADD(...) GetProcAddress(__VA_ARGS__)
#define FREEDL(...) FreeLibrary(__VA_ARGS__)
#define DLERROR() "Just Error"
#else
#include <dlfcn.h>
#define LOADDL(...) dlopen(__VA_ARGS__)
#define GETADD(...) dlsym(__VA_ARGS__)
#define FREEDL(...) dlclose(__VA_ARGS__)
#define DLERROR() dlerror()
#endif

namespace Sunset
{
    GameModuleLoader::GameModuleLoader()
        : m_Handle(nullptr)
        , module(nullptr)
        , m_Create(nullptr)
        , m_Destroy(nullptr)
    {
    }

    bool GameModuleLoader::Load(const std::filesystem::path &path, Application &app)
    {
        m_Handle = LOADDL(path.c_str(), RTLD_NOW | RTLD_GLOBAL);
        if (!m_Handle)
        {
            LOG("Engine", error, "Enable to load game module error : {}", DLERROR())
            return false;
        }

        m_Create = reinterpret_cast<CreateGameModuleFn>(GETADD(m_Handle, "SunsetCreateGameModule"));

        m_Destroy = reinterpret_cast<DestroyGameModuleFn>(GETADD(m_Handle, "SunsetDestroyGameModule"));

        if (!m_Create || !m_Destroy)
        {
            LOG("Engine", error, "Cannot load game module symbols from : {}", path.string())
            FREEDL(m_Handle);
            m_Handle = nullptr;
            m_Create = nullptr;
            m_Destroy = nullptr;
            return false;
        }

        if ((module = m_Create()))
            module->Load(app);
        else
        {
            LOG("Engine", error, "Game module factory returned null : {}", path.string())
            return false;
        }

        return true;
    }

    void GameModuleLoader::Unload()
    {
        if (module)
        {
            module->Unload();
            m_Destroy(module);
            module = nullptr;
        }

        if (m_Handle)
        {
            FREEDL(m_Handle);
            m_Handle = nullptr;
        }
    }
}
