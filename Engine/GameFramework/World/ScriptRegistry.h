//
// Created by sunvy on 16/09/2026.
//

#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <vector>

namespace Sunset
{
    class NativeScriptComponent;
    class ScriptEntity;

    struct ScriptRegistryEntry
    {
        std::string Name;
        std::function<ScriptEntity*()> Instantiate;
    };

    class ScriptRegistry final
    {
    public:
        template <typename T>
        requires(std::is_base_of_v<ScriptEntity, T>)
        static void Register(std::string name)
        {
            Register(std::move(name), []() -> ScriptEntity*
            {
                return new T();
            });
        }

        static void Register(std::string name, std::function<ScriptEntity*()> instantiate);
        static const std::vector<ScriptRegistryEntry>& GetScripts();
        static const ScriptRegistryEntry* Find(std::string_view name);
        static bool AddScriptTo(NativeScriptComponent& component, std::string_view name);
    };
} // Sunset
