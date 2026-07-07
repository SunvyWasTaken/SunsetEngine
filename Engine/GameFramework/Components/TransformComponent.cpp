//
// Created by sunvy on 06/06/2026.
//

#include "TransformComponent.h"

#include <glm/ext/matrix_transform.hpp>

#include "Network/NetworkService.h"

namespace
{
    struct NetworkTransformMessage : public Sunset::IMessage
    {
        static constexpr Sunset::ChannelId ChannelId = 2;
        Sunset::PeerId OwnerPeerId = 0;
        float LocationX = 0.0f;
        float LocationY = 0.0f;
        float LocationZ = 0.0f;
    };

    std::unordered_map<Sunset::PeerId, NetworkTransformMessage> networkTransforms;
    bool networkTransformHandlerRegistered = false;

    void EnsureNetworkTransformHandler()
    {
        if (networkTransformHandlerRegistered)
            return;

        if (Sunset::NetworkService::IsInitialized())
        {
            Sunset::NetworkService::Get().RegisterMessage<NetworkTransformMessage>(2);
            Sunset::NetworkService::Get().RegisterHandler<NetworkTransformMessage>([](Sunset::PeerId peer, const NetworkTransformMessage& msg)
            {
                NetworkTransformMessage transform = msg;
                if (transform.OwnerPeerId == 0)
                    transform.OwnerPeerId = peer;
                networkTransforms[transform.OwnerPeerId] = transform;
            });
        }

        networkTransformHandlerRegistered = true;
    }
}

namespace Sunset
{
    TransformComponent::TransformComponent(PeerId ownerPeerId, bool syncPositionInWorld, bool broadcastPositionInWorld)
    : OwnerPeerId(ownerPeerId)
    , bSyncPositionInWorld(syncPositionInWorld)
    , bBroadcastPositionInWorld(broadcastPositionInWorld)
    {
        EnsureNetworkTransformHandler();
    }

    void TransformComponent::Update(float deltatime)
    {
        (void)deltatime;

        EnsureNetworkTransformHandler();

        if (bSyncPositionInWorld)
        {
            if (const auto transform = networkTransforms.find(OwnerPeerId); transform != networkTransforms.end())
            {
                SetLocation({
                    transform->second.LocationX,
                    transform->second.LocationY,
                    transform->second.LocationZ
                });
            }
        }

        if (!bBroadcastPositionInWorld)
            return;

        const glm::vec3 location = GetLocation();
        NetworkTransformMessage msg;
        msg.OwnerPeerId = OwnerPeerId;
        msg.LocationX = location.x;
        msg.LocationY = location.y;
        msg.LocationZ = location.z;
        NetworkService::Get().Broadcast(msg, DeliveryType::Unreliable);
    }

    glm::vec3 TransformComponent::GetForwardVector() const
    {
        return glm::normalize(Rotation * glm::vec3(0.f, 0.f, -1.f));
    }

    glm::vec3 TransformComponent::GetRightVector() const
    {
        return glm::normalize(glm::cross(GetForwardVector(), {0, 1, 0}));
    }

    glm::vec3 TransformComponent::GetLocation() const
    {
        return Position;
    }

    glm::vec3 TransformComponent::GetScale() const
    {
        return Scale;
    }

    void TransformComponent::AddLocation(const glm::vec3 &location)
    {
        Position += location;
    }

    void TransformComponent::SetLocation(const glm::vec3 &location)
    {
        Position = location;
    }

    void TransformComponent::Rotate(const glm::vec3 &axis, float angle)
    {
        Rotation = glm::normalize(glm::angleAxis(angle, glm::normalize(axis)) * Rotation);
    }

    void TransformComponent::AddScale(const glm::vec3 &scale)
    {
        Scale += scale;
    }

    void TransformComponent::SetScale(const glm::vec3 &scale)
    {
    }
} // Sunset