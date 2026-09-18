//
// Created by sunvy on 06/06/2026.
//

#include "CameraComponent.h"

#include <glm/gtc/type_ptr.hpp>

#include "SaveSystem/BinaryArchive.h"

namespace Sunset
{
    void CameraComponent::SetProjectionType(const ProjectionType& projectionType)
    {
        camera.m_ProjectionType = projectionType;
    }

    ReflectionType CameraComponent::Properties()
    {
        ReflectionType properties;
        properties.Field("Activate", &CameraComponent::Primary);
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

    void Serialize(BinaryInputArchive& archive, CameraComponent& component)
    {
        archive(component.Primary);
        archive(component.camera);
    }

    void Serialize(BinaryOutputArchive& archive, CameraComponent& component)
    {
        archive(component.Primary);
        archive(component.camera);
    }

    void Serialize(BinaryInputArchive& archive, Camera& camera)
    {
        archive(camera.m_Position);
        archive(camera.m_Forward);
        archive(camera.m_Up);
        archive(camera.m_Yaw);
        archive(camera.m_Pitch);
        archive(camera.m_Fov);
        archive(camera.m_CameraDistance);
        archive(camera.OrthographicSize);
        archive(camera.NearPlaneDistance);
        archive(camera.FarPlaneDistance);
        archive(camera.m_ProjectionType);
    }

    void Serialize(BinaryOutputArchive& archive, Camera& camera)
    {
        archive(camera.m_Position);
        archive(camera.m_Forward);
        archive(camera.m_Up);
        archive(camera.m_Yaw);
        archive(camera.m_Pitch);
        archive(camera.m_Fov);
        archive(camera.m_CameraDistance);
        archive(camera.OrthographicSize);
        archive(camera.NearPlaneDistance);
        archive(camera.FarPlaneDistance);
        archive(camera.m_ProjectionType);
    }
} // Sunset
