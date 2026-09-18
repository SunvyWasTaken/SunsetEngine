//
// Runtime catalogue of components that can be attached to an entity.
//

#pragma once

#include <concepts>

#include "GameFramework/World/Entity.h"

namespace Sunset
{
    enum class ComponentRegistrySource
    {
        Engine,
        Project
    };

    struct ComponentRegistryEntry
    {
        std::string Name;
        std::string Category;
        entt::id_type TypeId = 0;
        ComponentRegistrySource Source = ComponentRegistrySource::Project;
        std::function<bool(const Entity&)> Has;
        std::function<bool(Entity&)> Add;
    };

    class ComponentRegistry final
    {
    public:
        template <typename T>
        requires std::derived_from<T, Component> && std::default_initializable<T>
        static bool Register(std::string name, std::string category)
        {
            EnsureEngineComponentsRegistered();

            return RegisterEntry({
                .Name = std::move(name),
                .Category = std::move(category),
                .TypeId = entt::type_hash<T>::value(),
                .Source = ComponentRegistrySource::Project,
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
        }

        [[nodiscard]] static const std::vector<ComponentRegistryEntry>& GetEntries();

        // Called by GameModuleLoader before unloading a game DLL.
        static void ClearProjectComponents();

    private:
        static void EnsureEngineComponentsRegistered();
        static bool RegisterEntry(ComponentRegistryEntry entry);
    };
} // Sunset
