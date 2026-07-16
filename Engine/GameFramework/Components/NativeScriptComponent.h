//
// Created by sunvy on 07/07/2026.
//

#pragma once
#include "Component.h"

namespace entt
{
    enum class entity : std::uint32_t;
}

namespace Sunset
{
    class Entity;
    class World;
    class ScriptEntity;

    class NativeScriptComponent : public Component
    {
    public:
        NativeScriptComponent();

        NativeScriptComponent(const NativeScriptComponent&) = delete;

        NativeScriptComponent& operator=(const NativeScriptComponent&) = delete;

        NativeScriptComponent(NativeScriptComponent&&) noexcept = default;

        NativeScriptComponent& operator=(NativeScriptComponent&&) noexcept = default;

        std::vector<std::unique_ptr<ScriptEntity>> m_ScriptEntitys;

        void Start(World* world, const Entity& entity);

        std::vector<std::function<ScriptEntity*()>> InstantiateScriptEntity;
        // void(*DestroyScriptEntity)(NativeScriptComponent*);

        template <typename T, typename ...Args>
        requires(std::is_base_of_v<ScriptEntity, T>)
        void Bind(Args&&... args)
        {
            using ArgsTuple = std::tuple<std::decay_t<Args>...>;
            InstantiateScriptEntity.emplace_back([storedArgs = ArgsTuple{std::forward<Args>(args)...}]() mutable
            {
                return std::apply(
                    []<typename... T0>(T0&&... unpackedArgs) -> ScriptEntity*
                    {
                        return new T(
                            std::forward<T0>(unpackedArgs)...
                        );
                    },
                    storedArgs
                );
            });
            // DestroyScriptEntity = [](NativeScriptComponent* self){ delete static_cast<T*>(self->m_ScriptEntitys); };
        }
    };
} // Sunset