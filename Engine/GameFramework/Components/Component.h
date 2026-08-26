//
// Created by sunvy on 03/06/2026.
//

#pragma once

#include <entt/entity/entity.hpp>

#include "Reflection/ReflectionType.h"

namespace Sunset
{
    class World;
    class Entity;

    struct Component
    {
    private:
        friend class Entity;
        World* m_world = nullptr;
        entt::entity m_owner = entt::null;
    public:
        Component() = default;
        virtual ~Component();

        Entity GetOwner() const;

        template <typename T>
        requires std::is_base_of_v<Component, T>
        T* GetComponent() const;

        virtual ReflectionType Properties();
    };

    struct TagComponent : public Component
    {
        std::string Tag;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        explicit TagComponent(const std::string& tag) : Tag(tag) {}

        ~TagComponent() override;

        ReflectionType Properties() override;
    };

    template <typename Archive>
    void Serialize(Archive& ar, TagComponent& component);
} // Sunset