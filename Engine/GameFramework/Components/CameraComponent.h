//
// Created by sunvy on 06/06/2026.
//

#pragma once
#include "Component.h"
#include "Render/Camera.h"

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

    template <typename Archive>
    void Serialize(Archive& ar, CameraComponent& component);
} // Sunset