//
// Created by sunvy on 03/06/2026.
//

#pragma once

#include "Render/Drawable.h"

namespace Sunset
{
    struct Component
    {
    };

    struct TagComponent : public Component
    {
        std::string Tag;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag) : Tag(tag) {}

    };

    struct TransformComponent : public Component
    {
        glm::mat4 Transform;
        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4& transform);

        glm::vec3 GetLocation() const;

        void AddLocation(const glm::vec3& location);
        void SetLocation(const glm::vec3& location);
        void AddScale(const glm::vec3& scale);
        void SetScale(const glm::vec3& scale);

        operator glm::mat4& () { return Transform; }
        operator const glm::mat4& () const { return Transform; }
    };

    struct MeshComponent : public Component
    {
        Drawable m_mesh;
    };
} // Sunset