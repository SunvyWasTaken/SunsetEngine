//
// Created by sunvy on 03/06/2026.
//

#include "Component.h"

#include "GameFramework/World/Entity.h"

namespace Sunset
{
    Component::~Component()
    {
    }

    Entity Component::GetOwner() const
    {
        return Entity{m_world, m_owner};
    }

    ReflectionType Component::Properties()
    {
        return {};
    }

    TagComponent::TagComponent()
    {
        LOG("Engine", trace, "Tag Component empty Create");
    }

    TagComponent::TagComponent(const TagComponent &)
    {
        LOG("Engine", trace, "Tag Component {} created", Tag);
    }

    TagComponent::TagComponent(const std::string &tag)
    {
        LOG("Engine", trace, "Tag Component {} created", tag);
    }

    TagComponent::~TagComponent()
    {
        LOG("Engine", trace, "Tag Component {} destroy", Tag);
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