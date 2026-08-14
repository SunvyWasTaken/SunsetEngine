//
// Created by sunvy on 03/06/2026.
//

#include "Component.h"

#include "GameFramework/World/Entity.h"

namespace Sunset
{
    Component::~Component()
    {
        LOG("Engine", trace, "Component::~Component()")
    }

    Entity Component::GetOwner() const
    {
        return Entity{m_world, m_owner};
    }

    ReflectionType Component::Properties()
    {
        return {};
    }

    ReflectionType TagComponent::Properties()
    {
        ReflectionType properties;
        properties.Field("Tag", &TagComponent::Tag);
        return properties;
    }

    template<typename Archive>
    void Serialize(Archive &ar, TagComponent &component)
    {
        ar(component.Tag);
    }
} // Sunset