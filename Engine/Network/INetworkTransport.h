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

        virtual bool Host(uint16_t port, size_t maxClients) = 0;

        virtual bool Connect(const std::string& address, uint16_t port) = 0;

        virtual void Disconnect(PeerId peer) = 0;

        virtual void Update() = 0;

        virtual void Send(PeerId peer, const void* data, size_t size, ChannelId channel, DeliveryType mode) = 0;

        virtual std::vector<NetworkEvent> PollEvents() = 0;
    };
}
