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
        Transform = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, 5.0f));
        Transform = glm::scale(Transform, glm::vec3(1.0f, 1.0f, 1.0f));
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
    }

    void TransformComponent::AddScale(const glm::vec3 &scale)
    {
        Transform = glm::scale(Transform, scale);
    }

    void TransformComponent::SetScale(const glm::vec3 &scale)
    {
    }
} // Sunset