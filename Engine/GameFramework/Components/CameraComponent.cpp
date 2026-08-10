//
// Created by sunvy on 06/06/2026.
//

#include "CameraComponent.h"

#include <glm/gtc/type_ptr.hpp>

namespace Sunset
{
    void CameraComponent::SetProjectionType(const ProjectionType& projectionType)
    {
        camera.m_ProjectionType = projectionType;
    }

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
        properties.Field("Position", [](void* instance)
        {
            auto* component = static_cast<CameraComponent*>(instance);
            return &component->camera.m_Position;
        });
        properties.Field("OrthographicSize", [](void* instance)
        {
            auto* component = static_cast<CameraComponent*>(instance);
            return &component->camera.OrthographicSize;
        });
        properties.Field("Near", [](void* instance)->float*
        {
            auto* component = static_cast<CameraComponent*>(instance);
            return &component->camera.NearPlaneDistance;
        });
        properties.Field("Far", [](void* instance)->float*
        {
            auto* component = static_cast<CameraComponent*>(instance);
            return &component->camera.FarPlaneDistance;
        });
        return properties;
    }

    void CameraComponent::Activate(const bool active)
    {
        Primary = active;
    }
} // Sunset
