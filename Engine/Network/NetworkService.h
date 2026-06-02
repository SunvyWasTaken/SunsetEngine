//
// Created by sunvy on 02/06/2026.
//

#pragma once

#include "NetworkType.h"

namespace Sunset
{
    class INetworkTransport;

    struct IMessage
    {};

    class NetworkService
    {
    public:

        NetworkService();

        ~NetworkService();

        static NetworkService& Get();

        bool Host(uint16_t port, uint32_t maxPlayer);
        bool Join(const EndPoint& endpoint);

        void Update(float dt);
        void Shutdown();

        void Send(
            PeerId peer,
            ChannelId channel,
            std::span<const std::byte> payload,
            DeliveryType mode = DeliveryType::Reliable
        );

        void Broadcast(
            ChannelId channel,
            std::span<const std::byte> payload,
            DeliveryType mode = DeliveryType::Reliable
        );

        template <typename T>
        void RegisterHandler(std::function<void(const T&)> handler)
        {

        }

    private:
        std::unique_ptr<INetworkTransport> m_Transport;
    };
}
