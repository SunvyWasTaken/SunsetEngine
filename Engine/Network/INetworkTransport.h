//
// Created by sunvy on 01/06/2026.
//

#pragma once
#include "NetworkEvent.h"
#include "NetworkType.h"

namespace Sunset
{
    class INetworkTransport
    {
    public:
        virtual ~INetworkTransport() = default;

        virtual bool StartServer(uint16_t port, uint32_t maxPeers) = 0;
        virtual bool Connect(const EndPoint& endpoint) = 0;
        virtual void Disconnect(PeerId peer) = 0;

        virtual void Send(
            PeerId peer,
            ChannelId channel,
            std::span<const std::byte> payload,
            DeliveryType mode
        ) = 0;

        virtual void Broadcast(
            ChannelId channel,
            std::span<const std::byte> payload,
            DeliveryType mode
        ) = 0;

        virtual std::vector<NetworkEvent::Type> PollEvents() = 0;
        virtual void Flush() = 0;
        virtual void Shutdown() = 0;
    };
}
