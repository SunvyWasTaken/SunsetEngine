//
// Runtime catalogue of components that can be attached to an entity.
//

#pragma once

#include <concepts>
#include <functional>
#include <string>
#include <string_view>
#include <vector>

#include "GameFramework/World/Entity.h"

namespace Sunset
{
    class BinaryInputArchive;
    class BinaryOutputArchive;

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
        std::function<void(const Entity&, Entity&)> Copy;
        std::function<void(BinaryOutputArchive&, Entity&)> Save;
        std::function<void(BinaryInputArchive&, Entity&)> Load;
    };

    template <typename T>
    struct ProjectComponentCallbacks
    {
        std::function<void(const T&, T&)> Copy;
        std::function<void(BinaryOutputArchive&, T&)> Save;
        std::function<void(BinaryInputArchive&, T&)> Load;
    };

    class ComponentRegistry final
    {
    public:
        template <typename T>
        requires std::derived_from<T, Component> && std::default_initializable<T>
        static bool Register(std::string name, std::string category, ProjectComponentCallbacks<T> callbacks)
        {
            EnsureEngineComponentsRegistered();

            if (!callbacks.Copy || !callbacks.Save || !callbacks.Load)
            {
                LOG("Engine", error, "Project component '{}' must provide copy, save, and load callbacks", name)
                return false;
            }

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
                },
                .Copy = [copy = std::move(callbacks.Copy)](const Entity& source, Entity& target)
                {
                    const auto* sourceComponent = source.GetComponent<T>();
                    if (!sourceComponent || target.GetComponent<T>())
                        return;

                    auto& targetComponent = target.AddComponent<T>();
                    copy(*sourceComponent, targetComponent);
                },
                .Save = [save = std::move(callbacks.Save)](BinaryOutputArchive& archive, Entity& entity)
                {
                    if (auto* component = entity.GetComponent<T>())
                        save(archive, *component);
                },
                .Load = [load = std::move(callbacks.Load)](BinaryInputArchive& archive, Entity& entity)
                {
                    if (entity.GetComponent<T>())
                        return;

                    auto& component = entity.AddComponent<T>();
                    load(archive, component);
                }
            });
        }

        [[nodiscard]] static const std::vector<ComponentRegistryEntry>& GetEntries();
        [[nodiscard]] static const ComponentRegistryEntry* FindProjectComponent(std::string_view name);

        // Called by GameModuleLoader before unloading a game DLL.
        static void ClearProjectComponents();

    private:
        static void EnsureEngineComponentsRegistered();
        static bool RegisterEntry(ComponentRegistryEntry entry);
    };
} // Sunset
