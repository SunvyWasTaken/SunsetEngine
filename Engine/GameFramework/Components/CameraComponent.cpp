//
// Created by sunvy on 06/06/2026.
//

#include "CameraComponent.h"

namespace Sunset
{
    ReflectionType CameraComponent::Properties()
    {
        ReflectionType properties;
        return properties;
    }

    void CameraComponent::Activate(const bool active)
    {
        Primary = active;
    }
} // Sunset