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

        template <typename T>
        [[nodiscard]] T CreateEntity()
        {
            static_assert(std::is_base_of_v<Entity, T>, "The class should be derived from Entity");
            T entity{this, m_Registry.create()};
            return entity;
        }

        entt::registry& Reg()
        {
            return m_Registry;
        }

    private:
        entt::registry m_Registry;
    };
} // Sunset