//
// Created by sunvy on 03/06/2026.
//

#pragma once

#include "entt/entt.hpp"

namespace Sunset
{
    class Entity;

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

    private:
        entt::registry m_Registry;
    };
} // Sunset