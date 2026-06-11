//
// Created by sunvy on 03/06/2026.
//

#pragma once

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
    public:
        World();

        virtual ~World();

        void Update(float deltatime);

        Entity CreateEntity(const std::string& name);

        entt::registry& Reg()
        {
            return m_Registry;
        }

        void OnPeerConnected(PeerId peerId);
        void OnPeerDisconnected(PeerId peerId);

    private:

        void OnPlayerSessionMessage(const NetworkPlayerSessionMessage& msg);
        void SetLocalPeerId(PeerId peerId);
        void DestroyPlayer(PeerId peer);
        void CreatePlayer(PeerId peer = 0, bool local = true);

    private:
        entt::registry m_Registry;
        std::vector<Controller> m_Controllers;
        PeerId m_LocalPeerId = 0;
    };
} // Sunset
