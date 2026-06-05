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

    private:

        void CreatePlayer(PeerId peer = 0);

    private:
        entt::registry m_Registry;
        std::vector<Controller> m_Controllers;
    };
} // Sunset