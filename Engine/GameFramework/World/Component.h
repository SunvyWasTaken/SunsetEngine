//
// Created by sunvy on 03/06/2026.
//

#pragma once
#include <glm/fwd.hpp>

namespace Sunset
{
    struct Component
    {
    };

    struct TransformComponent : public Component
    {
        glm::mat4 Transform;
        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4& transform);

        operator glm::mat4& () { return Transform; }
        operator const glm::mat4& () const { return Transform; }
    };
} // Sunset