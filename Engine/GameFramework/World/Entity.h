//
// Created by sunvy on 03/06/2026.
//

#pragma once

#include "GameFramework/Components/Component.h"
#include "World.h"

namespace Sunset
{
    class Entity
    {
    public:
        Entity(World* world = nullptr, entt::entity id = entt::null);

        virtual ~Entity();

        template <typename T, typename ...Args>
        T& AddComponent(Args&&... args)
        {
            static_assert(std::is_base_of_v<Component, T>, "The class should be derived from Component");
            return m_World->Reg().emplace<T>(m_Id, std::forward<Args>(args)...);
        }

        template <typename T>
        T* GetComponent() const
        {
            if (!m_World)
                return nullptr;

            return m_World->Reg().try_get<T>(m_Id);
        }

        template <typename T>
        void RemoveComponent()
        {
            m_World->Reg().remove<T>(m_Id);
        }

        explicit operator bool() const
        {
            return m_World != nullptr;
        }

        operator entt::entity() const
        {
            return m_Id;
        }

        operator std::uint32_t() const
        {
            return static_cast<std::uint32_t>(m_Id);
        }

        bool operator==(const Entity& other) const
        {
            return m_Id == other.m_Id && m_World == other.m_World;
        }

        bool operator!=(const Entity& other) const
        {
            return !(*this == other);
        }

    private:

        World* m_World;

        entt::entity m_Id;
    };
} // Sunset