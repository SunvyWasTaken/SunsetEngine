//
// Created by sunvy on 03/06/2026.
//

#pragma once

#include "Component.h"
#include "World.h"
#include "entt/entt.hpp"

namespace Sunset
{
    class Entity
    {
    public:
        Entity(World* world = nullptr, entt::entity id = entt::null);

        virtual ~Entity();

        template <typename T, typename ...Args>
        void AddComponent(Args&&... args)
        {
            static_assert(std::is_base_of_v<Component, T>, "The class should be derived from Component");
            if (!m_World)
                return;

            m_World->Reg().emplace<T>(m_Id, std::forward<Args>(args)...);
        }

        template <typename T>
        T* GetComponent() const
        {
            if (!m_World)
                return nullptr;

            return m_World->Reg().try_get<T>(m_Id);
        }

    private:

        World* m_World;

        entt::entity m_Id;
    };
} // Sunset