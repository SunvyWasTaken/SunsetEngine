//
// Created by sunvy on 18/09/2026.
//

#include "ComponentRegistry.h"

#include <algorithm>

#include "CameraComponent.h"
#include "InputComponent.h"
#include "NativeScriptComponent.h"
#include "SpriteRenderComponent.h"
#include "TransformComponent.h"
#include "GameFramework/World/ScriptEntity.h"

namespace
{
    std::vector<Sunset::ComponentRegistryEntry> s_Entries;
    bool s_EngineComponentsRegistered = false;

    bool CompareEntries(const Sunset::ComponentRegistryEntry& lhs, const Sunset::ComponentRegistryEntry& rhs)
    {
        if (lhs.Category != rhs.Category)
            return lhs.Category < rhs.Category;

        return lhs.Name < rhs.Name;
    }
}

namespace Sunset
{
    void ComponentRegistry::EnsureEngineComponentsRegistered()
    {
        if (s_EngineComponentsRegistered)
            return;

        s_EngineComponentsRegistered = true;

        const auto registerEngineComponent = []<typename T>(std::string name, std::string category)
        {
            RegisterEntry({
                .Name = std::move(name),
                .Category = std::move(category),
                .TypeId = entt::type_hash<T>::value(),
                .Source = ComponentRegistrySource::Engine,
                .Has = [](const Entity& entity)
                {
                    return entity.GetComponent<T>() != nullptr;
                },
                .Add = [](Entity& entity)
                {
                    if (entity.GetComponent<T>())
                        return false;

                    entity.AddComponent<T>();
                    return true;
                }
            });
        };

        registerEngineComponent.template operator()<TransformComponent>("Transform", "Core");
        registerEngineComponent.template operator()<CameraComponent>("Camera", "Rendering");
        registerEngineComponent.template operator()<SpriteRenderComponent>("Sprite Render", "Rendering");
        registerEngineComponent.template operator()<InputComponent>("Input", "Gameplay");
        registerEngineComponent.template operator()<NativeScriptComponent>("Native Script", "Scripting");
    }

    bool ComponentRegistry::RegisterEntry(ComponentRegistryEntry entry)
    {
        const auto duplicateName = std::ranges::find_if(s_Entries, [&entry](const ComponentRegistryEntry& current)
        {
            return current.Name == entry.Name && current.TypeId != entry.TypeId;
        });
        if (duplicateName != s_Entries.end())
        {
            LOG("Engine", error, "Component name '{}' is already registered by another type", entry.Name)
            return false;
        }

        const auto existing = std::ranges::find_if(s_Entries, [typeId = entry.TypeId](const ComponentRegistryEntry& current)
        {
            return current.TypeId == typeId;
        });

        if (existing != s_Entries.end())
        {
            *existing = std::move(entry);
            std::ranges::sort(s_Entries, CompareEntries);
            return false;
        }

        s_Entries.emplace_back(std::move(entry));
        std::ranges::sort(s_Entries, CompareEntries);
        return true;
    }

    const std::vector<ComponentRegistryEntry>& ComponentRegistry::GetEntries()
    {
        EnsureEngineComponentsRegistered();
        return s_Entries;
    }

    const ComponentRegistryEntry* ComponentRegistry::FindProjectComponent(const std::string_view name)
    {
        EnsureEngineComponentsRegistered();

        const auto entry = std::ranges::find_if(s_Entries, [name](const ComponentRegistryEntry& current)
        {
            return current.Source == ComponentRegistrySource::Project && current.Name == name;
        });
        return entry == s_Entries.end() ? nullptr : &*entry;
    }

    void ComponentRegistry::ClearProjectComponents()
    {
        EnsureEngineComponentsRegistered();
        std::erase_if(s_Entries, [](const ComponentRegistryEntry& entry)
        {
            return entry.Source == ComponentRegistrySource::Project;
        });
    }
} // Sunset
