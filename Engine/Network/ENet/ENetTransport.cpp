//
// Created by sunvy on 01/06/2026.
//

#include "ENetTransport.h"

#include <enet/enet.h>

namespace
{
    constexpr uint32_t kChannelCount = ENET_PROTOCOL_MAXIMUM_CHANNEL_COUNT;
    constexpr uint32_t kNoBandwidthLimit = 0;
    constexpr Sunset::PeerId kInvalidPeerId = 0;
}

namespace Sunset
{
    struct ENetTransport::Impl
    {
        explicit operator bool() const
        {
            return Host != nullptr;
        }

        PeerId RegisterPeer(ENetPeer* peer)
        {
            if (peer == nullptr)
                return kInvalidPeerId;

            const auto existingPeer = std::ranges::find_if(Peers, [peer](const auto& entry)
            {
                return entry.second == peer;
            });

            if (existingPeer != Peers.end())
                return existingPeer->first;

            const PeerId peerId = NextPeerId++;
            Peers[peerId] = peer;
            peer->data = reinterpret_cast<void*>(static_cast<uintptr_t>(peerId));
            return peerId;
        }

        PeerId GetPeerId(const ENetPeer* peer) const
        {
            if (peer == nullptr)
                return kInvalidPeerId;

            const auto existingPeer = std::ranges::find_if(Peers, [peer](const auto& entry)
            {
                return entry.second == peer;
            });

            if (existingPeer == Peers.end())
                return kInvalidPeerId;

            return existingPeer->first;
        }

        void UnregisterPeer(ENetPeer* peer)
        {
            const PeerId peerId = GetPeerId(peer);
            if (peerId == kInvalidPeerId)
                return;

            Peers.erase(peerId);
            if (peer != nullptr)
                peer->data = nullptr;
        }

        ENetHost* Host = nullptr;
        ENetPeer* ServerPeer = nullptr;
        std::unordered_map<PeerId, ENetPeer*> Peers;
        PeerId NextPeerId = 1;
        bool IsInitialized = false;
    };


    ENetTransport::ENetTransport()
    {
        LOG("Engine", info, "ENetTransport Create");
        m_Impl = std::make_unique<Impl>();
        if (enet_initialize() != 0)
        {
            LOG("Engine", error, "Failed to initialize ENet");
            return;
        }
        m_Impl->IsInitialized = true;
    }

    ENetTransport::~ENetTransport()
    {
        LOG("Engine", info, "ENetTransport Destroy");
        Shutdown();

        if (m_Impl->IsInitialized)
        {
            enet_deinitialize();
            m_Impl->IsInitialized = false;
        }
    }

    bool ENetTransport::StartServer(uint16_t port, uint32_t maxClients)
    {
        if (!m_Impl->IsInitialized)
            return false;

        Shutdown();

        ENetAddress address{};
        address.host = ENET_HOST_ANY;
        address.port = port;

        m_Impl->Host = enet_host_create(&address, maxClients, kChannelCount, kNoBandwidthLimit, kNoBandwidthLimit);
        if (m_Impl->Host == nullptr)
        {
            LOG("Engine", error, "Failed to create ENet server host on port {}", port);
            return false;
        }

        LOG("Engine", info, "ENet server started on port {}", port);
        return true;
    }

    bool ENetTransport::Connect(const EndPoint& endpoint)
    {
        if (!m_Impl->IsInitialized)
            return false;

        Shutdown();

        m_Impl->Host = enet_host_create(nullptr, 1, kChannelCount, kNoBandwidthLimit, kNoBandwidthLimit);
        if (m_Impl->Host == nullptr)
        {
            LOG("Engine", error, "Failed to create ENet client host");
            return false;
        }

        ENetAddress address{};
        if (enet_address_set_host(&address, endpoint.Address.c_str()) != 0)
        {
            LOG("Engine", error, "Failed to resolve ENet host {}", endpoint.Address);
            Shutdown();
            return false;
        }
        address.port = endpoint.Port;
        m_Impl->ServerPeer = enet_host_connect(m_Impl->Host, &address, kChannelCount, 0);
        if (m_Impl->ServerPeer == nullptr)
        {
            LOG("Engine", error, "Failed to connect to ENet host {}:{}", endpoint.Address, endpoint.Port);
            Shutdown();
            return false;
        }
        LOG("Engine", info, "ENet connection requested to {}:{}", endpoint.Address, endpoint.Port);
        return true;
    }

    void ENetTransport::Disconnect(PeerId peer)
    {
        if (!*m_Impl)
            return;

        auto peerIt = m_Impl->Peers.find(peer);
        if (peerIt == m_Impl->Peers.end() || peerIt->second == nullptr)
            return;

        enet_peer_disconnect(peerIt->second, 0);
    }

    void ENetTransport::Send(PeerId peer, ChannelId channel, std::span<const std::byte> payload, DeliveryType mode)
    {
        if (!*m_Impl)
            return;

        const auto peerIt = m_Impl->Peers.find(peer);
        if (peerIt == m_Impl->Peers.end() || peerIt->second == nullptr)
        {
            LOG("Engine", error, "Cannot send ENet packet: unknown peer {}", peer);
            return;
        }

        ENetPeer* enetPeer = peerIt->second;
        if (channel >= enetPeer->channelCount)
        {
            LOG("Engine", error, "Cannot send ENet packet: channel {} is not available for peer {}", channel, peer);
            return;
        }

        const enet_uint32 flags = mode == DeliveryType::Reliable ? ENET_PACKET_FLAG_RELIABLE : 0;
        ENetPacket* packet = enet_packet_create(payload.data(), payload.size(), flags);
        if (packet == nullptr)
        {
            LOG("Engine", error, "Failed to create ENet packet for peer {}", peer);
            return;
        }

        if (enet_peer_send(enetPeer, channel, packet) != 0)
        {
            LOG("Engine", error, "Failed to queue ENet packet for peer {}", peer);
            enet_packet_destroy(packet);
        }
    }

    void ENetTransport::Broadcast(ChannelId channel, std::span<const std::byte> payload, DeliveryType mode)
    {
        if (!*m_Impl)
            return;

        if (channel >= m_Impl->Host->channelLimit)
        {
            LOG("Engine", error, "Cannot broadcast ENet packet: channel {} is not available", channel);
            return;
        }

        const enet_uint32 flags = mode == DeliveryType::Reliable ? ENET_PACKET_FLAG_RELIABLE : 0;
        ENetPacket* packet = enet_packet_create(payload.data(), payload.size(), flags);
        if (packet == nullptr)
        {
            LOG("Engine", error, "Failed to create ENet broadcast packet");
            return;
        }

        LOG("Engine", info, "Broadcast ENet packet for peer {}", channel);
        enet_host_broadcast(m_Impl->Host, channel, packet);
    }

    std::vector<NetworkEvent::Type> ENetTransport::PollEvents()
    {
        if (!*m_Impl)
            return {};

        std::vector<NetworkEvent::Type> events;
        ENetEvent event{};
        while (enet_host_service(m_Impl->Host, &event, 0) > 0)
        {
            switch (event.type)
            {
                case ENET_EVENT_TYPE_CONNECT:
                {
                    const PeerId peerId = m_Impl->RegisterPeer(event.peer);
                    LOG("Engine", info, "ENet peer {} connected", peerId);
                    events.emplace_back(NetworkEvent::PeerConnected{ .Peer = peerId });
                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT:
                {
                    const PeerId peerId = m_Impl->GetPeerId(event.peer);
                    LOG("Engine", info, "ENet peer {} disconnected", peerId);
                    events.emplace_back(NetworkEvent::PeerDisconnected{ .Peer = peerId });
                    m_Impl->UnregisterPeer(event.peer);
                    break;
                }
                case ENET_EVENT_TYPE_RECEIVE:
                {
                    const PeerId peerId = m_Impl->GetPeerId(event.peer);
                    Packet packet;
                    packet.peer = peerId;
                    packet.channel = event.channelID;
                    packet.payload.resize(event.packet->dataLength);
                    if (event.packet->dataLength > 0)
                        std::memcpy(packet.payload.data(), event.packet->data, event.packet->dataLength);
                    enet_packet_destroy(event.packet);

                    events.emplace_back(NetworkEvent::PacketReceived{ .packet = std::move(packet) });
                    break;
                }
                case ENET_EVENT_TYPE_NONE:
                default:
                    break;
            }
        }
        return events;
    }

    void ENetTransport::Flush()
    {
        if (!*m_Impl)
            return;

        enet_host_flush(m_Impl->Host);
    }

    void ENetTransport::Shutdown()
    {
        if (!m_Impl || m_Impl->Host == nullptr)
            return;

        for (const auto& [_, peer] : m_Impl->Peers)
        {
            if (peer != nullptr)
                enet_peer_disconnect_now(peer, 0);
        }

        enet_host_destroy(m_Impl->Host);
        m_Impl->Host = nullptr;
        m_Impl->ServerPeer = nullptr;
        m_Impl->Peers.clear();
        m_Impl->NextPeerId = 1;
    }

}
