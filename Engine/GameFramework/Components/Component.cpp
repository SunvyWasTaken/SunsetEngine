//
// Created by sunvy on 03/06/2026.
//

#include "Component.h"

namespace Sunset
{
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
} // Sunset