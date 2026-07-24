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
        requires(std::is_base_of_v<Component, T>)
        T& AddComponent(Args&&... args)
        {
            auto& comp = m_World->m_Registry.emplace<T>(m_Id, std::forward<Args>(args)...);
            comp.m_world = m_World;
            comp.m_owner = m_Id;
            return comp;
        }

        template <typename T>
        T* GetComponent() const
        {
            if (!m_World)
                return nullptr;

            return m_World->m_Registry.try_get<T>(m_Id);
        }

        template <typename T>
        void RemoveComponent() const
        {
            m_World->m_Registry.remove<T>(m_Id);
        }

        void Destroy();

        explicit operator bool() const
        {
            return m_World != nullptr && m_Id != entt::null;
        }

        explicit operator entt::entity() const
        {
            return m_Id;
        }

        explicit operator std::uint32_t() const
        {
            return static_cast<std::uint32_t>(m_Id);
        }

        [[nodiscard]]
        World* GetWorld() const
        {
            return m_World;
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

    template <typename T>
    requires std::is_base_of_v<Component, T>
    T* Component::GetComponent() const
    {
        return GetOwner().GetComponent<T>();
    }
} // Sunset
