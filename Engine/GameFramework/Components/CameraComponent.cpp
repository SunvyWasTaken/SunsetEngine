//
// Created by sunvy on 06/06/2026.
//

#include "CameraComponent.h"

namespace Sunset
{
    CameraComponent::CameraComponent(RenderPassMask renderMask, uint32_t frameBufferId, bool active)
        : camera(renderMask, frameBufferId, active)
    {
    }
} // Sunset
