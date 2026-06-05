//
// Created by sunvy on 03/06/2026.
//

#include "Component.h"

#include <glm/ext/matrix_transform.hpp>

namespace Sunset
{
    TransformComponent::TransformComponent(const glm::mat4 &transform)
        : Transform(transform)
    {
    }

    glm::vec3 TransformComponent::GetLocation() const
    {
        return {Transform[3]};
    }

    void TransformComponent::AddLocation(const glm::vec3 &location)
    {
        Transform = glm::translate(Transform, location);
    }

    void TransformComponent::SetLocation(const glm::vec3 &location)
    {
        Transform[3] = glm::vec4(location, 1.0f);
    }

    void TransformComponent::AddScale(const glm::vec3 &scale)
    {
        Transform = glm::scale(Transform, scale);
    }

    void TransformComponent::SetScale(const glm::vec3 &scale)
    {
    }
} // Sunset