//
// Created by sunvy on 01/06/2026.
//

#include "ENetTransport.h"

#include <enet/enet.h>

namespace Sunset
{
    ENetTransport::ENetTransport()
    {
    }

    ENetTransport::~ENetTransport()
    {
    }

    bool ENetTransport::Host(uint16_t port, size_t maxClients)
    {
        ENetAddress address;
        address.host = ENET_HOST_ANY;
        address.port = port;

        m_Host = enet_host_create(&address, maxClients, 2, 0, 0);

        return m_Host != nullptr;
    }

    bool ENetTransport::Connect(const std::string &addressStr, uint16_t port)
    {
        ENetAddress address;
        enet_address_set_host(&address, addressStr.c_str());
        address.port = port;

        ENetPeer* peer = enet_host_connect(m_Host, &address, 2, 0);

        return peer != nullptr;
    }

    void ENetTransport::Disconnect(PeerId peer)
    {
    }

    void ENetTransport::Update()
    {
        ENetEvent event;

        while (enet_host_service(
            m_Host,
            &event,
            0) > 0)
        {
            switch (event.type)
            {
                case ENET_EVENT_TYPE_CONNECT:
                {
                    PeerId id = AllocatePeerId();

                    m_Peers[id] = event.peer;
                    m_PeerIds[event.peer] = id;

                    m_Events.push_back(
                        PeerConnected{id});

                    break;
                }

                case ENET_EVENT_TYPE_DISCONNECT:
                {
                    auto it =
                        m_PeerIds.find(event.peer);

                    if (it != m_PeerIds.end())
                    {
                        PeerId id = it->second;

                        m_Peers.erase(id);
                        m_PeerIds.erase(it);

                        m_Events.push_back(
                            PeerDisconnected{id});
                    }

                    break;
                }

                case ENET_EVENT_TYPE_RECEIVE:
                {
                    auto it =
                        m_PeerIds.find(event.peer);

                    if (it != m_PeerIds.end())
                    {
                        PacketReceived packet;

                        packet.Peer = it->second;

                        packet.Data.resize(
                            event.packet->dataLength);

                        std::memcpy(
                            packet.Data.data(),
                            event.packet->data,
                            event.packet->dataLength);

                        m_Events.push_back(
                            std::move(packet));
                    }

                    enet_packet_destroy(
                        event.packet);

                    break;
                }

                default:
                    break;
            }
        }
    }

    void ENetTransport::Send(PeerId peer, const void *data, size_t size, ChannelId channel, DeliveryType mode)
    {
        auto it = m_Peers.find(peer);

        if (it == m_Peers.end())
            return;

        ENetPacket* packet =
            enet_packet_create(
                data,
                size,
                mode == DeliveryType::Reliable
                    ? ENET_PACKET_FLAG_RELIABLE
                    : 0);

        enet_peer_send(
            it->second,
            channel,
            packet);
    }

    std::vector<NetworkEvent> ENetTransport::PollEvents()
    {
        return {};
    }
}
