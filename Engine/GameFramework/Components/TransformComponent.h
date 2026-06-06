//
// Created by sunvy on 06/06/2026.
//

#pragma once

#include "Component.h"

namespace Sunset
{
    struct TransformComponent : public Component
    {
        glm::mat4 Transform{1.f};

        TransformComponent() = default;

        TransformComponent(const TransformComponent&) = default;

        TransformComponent(const glm::mat4& transform);

        glm::vec3 GetForwardVector() const;

        glm::vec3 GetLocation() const;

        glm::vec3 GetScale() const;

        void AddLocation(const glm::vec3& location);

        void SetLocation(const glm::vec3& location);

        void Rotate(const glm::vec3& rotation, float angle);

        void AddScale(const glm::vec3& scale);

        void SetScale(const glm::vec3& scale);

        operator glm::mat4& () { return Transform; }

        operator const glm::mat4& () const { return Transform; }
    };
} // Sunset