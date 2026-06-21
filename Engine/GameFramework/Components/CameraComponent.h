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
        explicit CameraComponent(RenderPassMask renderMask, uint32_t frameBufferId = 0, bool active = true);
        ~CameraComponent() override = default;

        Camera camera;
    };
} // Sunset
