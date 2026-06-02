//
// Created by sunvy on 01/06/2026.
//

#pragma once

#include "Network/INetworkTransport.h"

namespace Sunset
{
    class ENetTransport : public INetworkTransport
    {
    public:
        ENetTransport();

        ~ENetTransport() override;

        bool StartServer(uint16_t port, uint32_t maxPeers) override;

        bool Connect(const EndPoint& endpoint) override;

        void Disconnect(PeerId peer) override;

        void Send(
            PeerId peer,
            ChannelId channel,
            std::span<const std::byte> payload,
            DeliveryType mode
        ) override;

        void Broadcast(
            ChannelId channel,
            std::span<const std::byte> payload,
            DeliveryType mode
        ) override;

        std::vector<NetworkEvent::Type> PollEvents() override;

        void Flush() override;

        void Shutdown() override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_Impl;
    };
}
