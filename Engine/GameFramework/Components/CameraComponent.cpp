//
// Created by sunvy on 06/06/2026.
//

#include "CameraComponent.h"

namespace Sunset
{
    ReflectionType CameraComponent::Properties()
    {
        ReflectionType properties;
        properties.EnumField<CameraComponent, ProjectionType>("Projection Type", [](void* instance) -> ProjectionType*
        {
            auto* component = static_cast<CameraComponent*>(instance);
            return &component->camera.m_ProjectionType;
        }, {
            {ProjectionType::Perspective, "Perspective"},
            {ProjectionType::Orthographic, "Orthographic"}
        });
        return properties;
    }

    void CameraComponent::Activate(const bool active)
    {
        Primary = active;
    }
} // Sunset
