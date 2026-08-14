//
// Created by sunvy on 03/06/2026.
//

#pragma once

#include "Core/Event.h"
#include "GameFramework/Components/Component.h"
#include "entt/entt.hpp"
#include "SaveSystem/BinaryArchive.h"

namespace Sunset
{
    class IWorldSystem;
    class Controller;
    class Entity;

    using PeerId = uint32_t;

    struct NetworkPlayerSessionMessage
    {
        static constexpr uint8_t ChannelId = 3;

        enum class Type : uint8_t
        {
            AssignLocalPeer = 0,
            PlayerJoined = 1,
            PlayerLeft = 2,
        };

        Type MessageType = Type::PlayerJoined;
        PeerId Peer = 0;
    };

    class World
    {
        friend class Entity;
        friend void Serialize(BinaryInputArchive& archive, World& world);
        friend void Serialize(BinaryOutputArchive& archive, World& world);
    public:
        World();

        virtual ~World();

        void BeginInput();

        bool OnEvent(const Event::Type& event);

        void Update(float dt);

        template <typename T>
        requires std::is_base_of_v<IWorldSystem, T>
        void AddSystem()
        {
            auto system = std::make_unique<T>(this);
            m_Systems.push_back(std::move(system));
        }

        Entity FindEntityByName(const std::string& name);

        template <typename Func>
        void ForEach(Func&& func)
        {
            for (const auto view = m_Registry.view<entt::entity>(); auto entity : view)
            {
                func({this, entity});
            }
        }

        template <typename... Components, typename Func>
        void Each(Func&& func)
        {
            const auto view = m_Registry.view<Components...>();

            for (auto entity : view)
            {
                func({this, entity}, view.template get<Components>(entity)...);
            }
        }

        template <typename Func>
        void ForEachComponent(const entt::entity& entity, Func&& func)
        {
            for (auto [componentTypeId, storage] : m_Registry.storage())
            {
                if (!storage.contains(entity))
                    continue;

                auto* component = static_cast<Component*>(storage.value(entity));
                func(componentTypeId, storage.info(), *component);
            }
        }

        Entity CreateEntity(const std::string& name);
        void DestroyEntity(const Entity& entity);

    private:
        entt::registry m_Registry;
        PeerId m_LocalPeerId = 0;
        std::vector<std::unique_ptr<IWorldSystem>> m_Systems;
    };

    template <typename Archive>
    void Serialize(Archive& ar, World& world);
} // Sunset
