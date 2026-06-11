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

        /// Starts the local process as a server listening on the given port.
        virtual bool StartServer(uint16_t port, uint32_t maxPeers) = 0;

        /// Starts the local process as a client and begins connecting to the target endpoint.
        virtual bool Connect(const EndPoint& endpoint) = 0;

        /// Requests a clean disconnection from a specific peer.
        virtual void Disconnect(PeerId peer) = 0;

        /// Queues a packet for one peer on the selected channel with the requested delivery mode.
        virtual void Send(
            PeerId peer,
            ChannelId channel,
            std::span<const std::byte> payload,
            DeliveryType mode
        ) = 0;

        /// Queues a packet for every connected peer on the selected channel.
        virtual void Broadcast(
            ChannelId channel,
            std::span<const std::byte> payload,
            DeliveryType mode
        ) = 0;

        /// Polls the backend once and returns the network events collected during this tick.
        virtual std::vector<NetworkEvent::Type> PollEvents() = 0;

        /// Forces queued outgoing packets to be submitted to the transport backend immediately.
        virtual void Flush() = 0;

        /// Releases the current host/client state and disconnects active peers.
        virtual void Shutdown() = 0;
    };
}
