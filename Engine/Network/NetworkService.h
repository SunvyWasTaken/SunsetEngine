//
// Created by sunvy on 02/06/2026.
//

#pragma once

#include <typeindex>

#include "NetworkType.h"

namespace Sunset
{
    class INetworkTransport;

    struct IMessage
    {};

    namespace Detail
    {
        template <typename T>
        concept HasStaticChannelId = requires
        {
            { T::ChannelId } -> std::convertible_to<Sunset::ChannelId>;
        };
    }

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

        void RegisterPeerConnectedHandler(std::function<void(PeerId)> handler);
        void RegisterPeerDisconnectedHandler(std::function<void(PeerId)> handler);

        void Send(
            PeerId peer,
            ChannelId channel,
            std::span<const std::byte> payload,
            DeliveryType mode = DeliveryType::Reliable
        );

        template <typename T>
        void Send(PeerId peer, const T& message, DeliveryType mode = DeliveryType::Reliable)
        {
            static_assert(std::is_trivially_copyable_v<T>, "Network message must be trivially copyable");

            const auto payload = std::as_bytes(std::span{&message, 1});
            Send(peer, ResolveChannel<T>(), payload, mode);
        }

        void Broadcast(
            ChannelId channel,
            std::span<const std::byte> payload,
            DeliveryType mode = DeliveryType::Reliable
        );

        template <typename T>
        void Broadcast(const T& message, DeliveryType mode = DeliveryType::Reliable)
        {
            static_assert(std::is_trivially_copyable_v<T>, "Network message must be trivially copyable");

            const auto payload = std::as_bytes(std::span{&message, 1});
            Broadcast(ResolveChannel<T>(), payload, mode);
        }

        template <typename T>
        void RegisterHandler(std::function<void(PeerId, const T&)> handler)
        {
            static_assert(std::is_trivially_copyable_v<T>, "Network message must be trivially copyable");

            const ChannelId channel = ResolveChannel<T>();
            m_Handlers[channel] = [channel, handler = std::move(handler)](PeerId peer, std::span<const std::byte> payload)
            {
                if (payload.size() != sizeof(T))
                {
                    LOG("Engine", error, "Invalid network payload size {} for channel {}, (expected {})", payload.size(), channel, sizeof(T))
                    return;
                }
                T message;
                std::memcpy(&message, payload.data(), sizeof(T));
                handler(peer, message);
            };
        }

    private:

        template <typename T>
        ChannelId ResolveChannel()
        {
            if constexpr (Detail::HasStaticChannelId<T>)
                return static_cast<ChannelId>(T::ChannelId);
            else
                return ResolveDynamicChannel<T>();
        }

        template <typename T>
        ChannelId ResolveDynamicChannel()
        {
            const std::type_index type = typeid(T);
            const auto existingChannel = m_TypeChannels.find(type);
            if (existingChannel != m_TypeChannels.end())
                return existingChannel->second;

            const ChannelId channel = m_NextDynamicChannel++;
            m_TypeChannels.emplace(type, channel);
            return channel;
        }

        void Dispatch(PeerId peer, ChannelId channel, std::span<const std::byte> payload);

    private:
        std::unique_ptr<INetworkTransport> m_Transport;
        std::unordered_map<ChannelId, std::function<void(PeerId, std::span<const std::byte>)>> m_Handlers;
        std::unordered_map<std::type_index, ChannelId> m_TypeChannels;
        std::vector<std::function<void(PeerId)>> m_PeerConnectedHandlers;
        std::vector<std::function<void(PeerId)>> m_PeerDisconnectedHandlers;
        ChannelId m_NextDynamicChannel = 0;
    };
}
