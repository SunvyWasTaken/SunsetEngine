//
// Created by sunvy on 06/06/2026.
//

#pragma once
#include "Component.h"
#include "Render/Resources/Camera.h"

namespace Sunset
{
    class CameraComponent : public Component
    {
    public:
        CameraComponent() = default;
        ~CameraComponent() override = default;

        void SetProjectionType(const ProjectionType& projectionType);

        ReflectionType Properties() override;

        void Activate(bool active);

        bool Primary = false;

        Camera camera;
    };

    void Serialize(BinaryInputArchive& archive, CameraComponent& component);

    void Serialize(BinaryOutputArchive& archive, CameraComponent& component);
} // Sunset