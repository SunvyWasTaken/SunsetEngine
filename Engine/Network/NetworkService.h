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
        /// Detects message types that explicitly declare the network channel they use.
        template <typename T>
        concept HasStaticChannelId = requires
        {
            { T::ChannelId } -> std::convertible_to<Sunset::ChannelId>;
        };
    }

    class NetworkMessageRegistry
    {
    public:
        /// Registers a message type on an explicit channel shared by the whole network protocol.
        template <typename T>
        bool Register(ChannelId channel)
        {
            static_assert(std::is_trivially_copyable_v<T>, "Network message must be trivially copyable");
            return Register(typeid(T), channel, typeid(T).name());
        }

        /// Finds the deterministic channel previously registered for the message type.
        template <typename T>
        bool TryResolve(ChannelId& channel) const
        {
            const auto channelIt = m_TypeChannels.find(typeid(T));
            if (channelIt == m_TypeChannels.end())
                return false;

            channel = channelIt->second;
            return true;
        }

    private:
        /// Stores the type/channel mapping and rejects conflicting protocol declarations.
        bool Register(std::type_index type, ChannelId channel, std::string_view messageName)
        {
            const auto typeIt = m_TypeChannels.find(type);
            if (typeIt != m_TypeChannels.end())
            {
                if (typeIt->second == channel)
                    return true;

                LOG("Engine", error, "Network message {} is already registered on channel {}", messageName, typeIt->second)
                return false;
            }

            const auto channelIt = m_ChannelTypes.find(channel);
            if (channelIt != m_ChannelTypes.end() && channelIt->second != type)
            {
                LOG("Engine", error, "Network channel {} is already registered by another message type", channel)
                return false;
            }

            m_TypeChannels.emplace(type, channel);
            m_ChannelTypes.emplace(channel, type);
            return true;
        }

    private:
        std::unordered_map<std::type_index, ChannelId> m_TypeChannels;
        std::unordered_map<ChannelId, std::type_index> m_ChannelTypes;
    };

    class NetworkService
    {
    public:

        NetworkService();

        ~NetworkService();

        /// Creates the global network service instance used by Get().
        static void Init();

        /// Destroys the global network service instance and releases its transport.
        static void Shutdown();

        static bool IsInitialized();

        /// Returns the global network service instance, creating it lazily if needed.
        static NetworkService& Get();

        /// Starts hosting a server on the given port for the requested number of players.
        bool Host(uint16_t port, uint32_t maxPlayer);

        /// Starts a client connection toward the provided endpoint.
        bool Join(const EndPoint& endpoint);

        /// Polls network events, dispatches packets, and calls connection callbacks.
        void Update(float dt);

        /// Registers a callback invoked when a peer connects.
        void RegisterPeerConnectedHandler(std::function<void(PeerId)> handler);

        /// Registers a callback invoked when a peer disconnects.
        void RegisterPeerDisconnectedHandler(std::function<void(PeerId)> handler);

        /// Registers a deterministic message/channel mapping shared by client and server code.
        template <typename T>
        bool RegisterMessage(ChannelId channel)
        {
            return m_MessageRegistry.Register<T>(channel);
        }

        /// Serializes a trivially copyable message and sends it to one peer.
        template <typename T>
        void Send(PeerId peer, const T& message, DeliveryType mode = DeliveryType::Reliable)
        {
            static_assert(std::is_trivially_copyable_v<T>, "Network message must be trivially copyable");

            ChannelId channel = 0;
            if (!TryResolveChannel<T>(channel))
                return;

            const auto payload = std::as_bytes(std::span{&message, 1});
            Send(peer, channel, payload, mode);
        }

        /// Sends an already serialized payload to every connected peer on the selected channel.
        void Broadcast(
            ChannelId channel,
            std::span<const std::byte> payload,
            DeliveryType mode = DeliveryType::Reliable
        );

        /// Serializes a trivially copyable message and broadcasts it to every connected peer.
        template <typename T>
        void Broadcast(const T& message, DeliveryType mode = DeliveryType::Reliable)
        {
            static_assert(std::is_trivially_copyable_v<T>, "Network message must be trivially copyable");

            ChannelId channel = 0;
            if (!TryResolveChannel<T>(channel))
                return;

            const auto payload = std::as_bytes(std::span{&message, 1});
            Broadcast(channel, payload, mode);
        }

        /// Registers a typed packet callback for the channel associated with the message type.
        template <typename T>
        void RegisterHandler(std::function<void(PeerId, const T&)> handler)
        {
            static_assert(std::is_trivially_copyable_v<T>, "Network message must be trivially copyable");

            ChannelId channel = 0;
            if (!TryResolveChannel<T>(channel))
                return;

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

        /// Sends an already serialized payload to one peer on the selected channel.
        void Send(
            PeerId peer,
            ChannelId channel,
            std::span<const std::byte> payload,
            DeliveryType mode = DeliveryType::Reliable
        );

        /// Resolves a message channel from a static declaration or the deterministic registry.
        template <typename T>
        bool TryResolveChannel(ChannelId& channel)
        {
            if constexpr (Detail::HasStaticChannelId<T>)
            {
                channel = static_cast<ChannelId>(T::ChannelId);
                return true;
            }
            else if (m_MessageRegistry.TryResolve<T>(channel))
            {
                return true;
            }
            LOG("Engine", error, "Network message {} has no deterministic channel. Register it with NetworkService::RegisterMessage<T>(channel) or add T::ChannelId", typeid(T).name())
            DEBUG_BREAK();
            return false;
        }

        /// Finds the handler registered for a channel and forwards the raw payload to it.
        void Dispatch(PeerId peer, ChannelId channel, std::span<const std::byte> payload);

    private:
        std::unique_ptr<INetworkTransport> m_Transport;
        NetworkMessageRegistry m_MessageRegistry;
        std::unordered_map<ChannelId, std::function<void(PeerId, std::span<const std::byte>)>> m_Handlers;
        std::vector<std::function<void(PeerId)>> m_PeerConnectedHandlers;
        std::vector<std::function<void(PeerId)>> m_PeerDisconnectedHandlers;
    };
}
