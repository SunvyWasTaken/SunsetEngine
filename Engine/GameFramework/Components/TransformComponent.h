//
// Created by sunvy on 06/06/2026.
//

#pragma once

#include <glm/detail/type_quat.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Component.h"

namespace Sunset
{
    using PeerId = uint32_t;

    struct TransformComponent : public Component
    {
        glm::vec3 Position{0.f};
        glm::quat Rotation{1.f, 0.f, 0.f, 0.f};
        glm::vec3 Scale{1.f};
        PeerId OwnerPeerId = 0;
        bool bSyncPositionInWorld = true;
        bool bBroadcastPositionInWorld = true;

        TransformComponent() = default;

        TransformComponent(const TransformComponent&) = default;
        TransformComponent(PeerId ownerPeerId, bool syncPositionInWorld = true, bool broadcastPositionInWorld = true);

        ReflectionType Properties() override;

        [[nodiscard]] glm::mat4 GetWorldMatrix() const noexcept;

        void Update(float deltatime);

        glm::vec3 GetForwardVector() const;

        glm::vec3 GetRightVector() const;

        glm::vec3 GetLocation() const;

        glm::vec3 GetScale() const;

        void AddLocation(const glm::vec3& location);

        void SetLocation(const glm::vec3& location);

        void Rotate(const glm::vec3& axis, float angle);

        void AddScale(const glm::vec3& scale);

        void SetScale(const glm::vec3& scale);

        operator glm::mat4 () const { return glm::translate(glm::mat4(1.f), Position) * glm::mat4_cast(Rotation) * glm::scale(glm::mat4(1.f), Scale); }
    };

    template <typename Archive>
    void Serialize(Archive& ar, TransformComponent& component);
} // Sunset