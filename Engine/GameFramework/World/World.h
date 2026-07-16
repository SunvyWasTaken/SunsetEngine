//
// Created by sunvy on 03/06/2026.
//

#pragma once

#include "Core/Event.h"
#include "entt/entt.hpp"

namespace Sunset
{
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
        friend class WorldHierarchyPanel;
    public:
        World();

        virtual ~World();

        bool OnEvent(const Event::Type& event);

        void Update(float dt);

        void Draw();

        Entity CreateEntity(const std::string& name);

        void OnPeerConnected(PeerId peerId);
        void OnPeerDisconnected(PeerId peerId);

    private:

        void OnPlayerSessionMessage(const NetworkPlayerSessionMessage& msg);

    private:
        entt::registry m_Registry;
        PeerId m_LocalPeerId = 0;
    };
} // Sunset
