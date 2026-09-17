//
// Created by sunvy on 16/09/2026.
//

#include "ScriptRegistry.h"

#include "GameFramework/Components/NativeScriptComponent.h"

namespace
{
    std::vector<Sunset::ScriptRegistryEntry> s_Scripts;
}

namespace Sunset
{
    void ScriptRegistry::Register(std::string name, std::function<ScriptEntity*()> instantiate)
    {
        if (auto* entry = const_cast<ScriptRegistryEntry*>(Find(name)))
        {
            entry->Instantiate = std::move(instantiate);
            return;
        }

        s_Scripts.emplace_back(std::move(name), std::move(instantiate));
    }

    const std::vector<ScriptRegistryEntry>& ScriptRegistry::GetScripts()
    {
        return s_Scripts;
    }

    const ScriptRegistryEntry* ScriptRegistry::Find(std::string_view name)
    {
        const auto it = std::ranges::find_if(s_Scripts, [name](const ScriptRegistryEntry& entry)
        {
            return entry.Name == name;
        });

        return it == s_Scripts.end() ? nullptr : &(*it);
    }

    bool ScriptRegistry::AddScriptTo(NativeScriptComponent& component, std::string_view name)
    {
        const ScriptRegistryEntry* entry = Find(name);
        if (!entry)
            return false;

        component.AddRegisteredScript(entry->Name, entry->Instantiate);
        return true;
    }
} // Sunset
