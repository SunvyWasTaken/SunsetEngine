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
    class BinaryInputArchive;
    class BinaryOutputArchive;

    class NativeScriptComponent : public Component
    {
    public:
        NativeScriptComponent();

        NativeScriptComponent(const NativeScriptComponent&) = delete;

        NativeScriptComponent& operator=(const NativeScriptComponent&) = delete;

        NativeScriptComponent(NativeScriptComponent&&) noexcept = default;

        NativeScriptComponent& operator=(NativeScriptComponent&&) noexcept = default;

        ~NativeScriptComponent() override;

        std::vector<std::unique_ptr<ScriptEntity>> m_ScriptEntitys;

        void Start(const Entity& entity);
        void Update(float dt);
        void Stop();
        void AddRegisteredScript(const std::string& name, std::function<ScriptEntity*()> instantiate);
        const std::vector<std::string>& GetRegisteredScriptNames() const;
        void CopyConfigurationTo(NativeScriptComponent& target) const;

        std::vector<std::function<ScriptEntity*()>> InstantiateScriptEntity;
        std::vector<std::string> RegisteredScriptNames;

        template <typename T, typename ...Args>
        requires(std::is_base_of_v<ScriptEntity, T>)
        void Bind(Args&&... args)
        {
            using ArgsTuple = std::tuple<std::decay_t<Args>...>;
            AddScriptFactory([storedArgs = ArgsTuple{std::forward<Args>(args)...}]() mutable
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
        }

    private:
        friend void Serialize(BinaryInputArchive& archive, NativeScriptComponent& component);
        friend void Serialize(BinaryOutputArchive& archive, NativeScriptComponent& component);

        void AddScriptFactory(std::function<ScriptEntity*()> instantiate);
        void InstantiateMissingScripts(const Entity& entity);

        std::vector<std::size_t> m_RegisteredScriptIndices;
        std::vector<bool> m_ScriptStarted;
    };

    void Serialize(BinaryInputArchive& archive, NativeScriptComponent& component);
    void Serialize(BinaryOutputArchive& archive, NativeScriptComponent& component);
} // Sunset
