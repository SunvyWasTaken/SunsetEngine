//
// Created by sunvy on 01/06/2026.
//

#pragma once

#include "Network/INetworkTransport.h"

struct _ENetHost;
struct _ENetPeer;

using ENetHost = _ENetHost;
using ENetPeer = _ENetPeer;

namespace Sunset
{
    class ENetTransport : public INetworkTransport
    {
    public:
        ENetTransport();

        ~ENetTransport() override;

        bool Host(uint16_t port, size_t maxClients) override;

        bool Connect(const std::string &addressStr, uint16_t port) override;

        void Disconnect(PeerId peer) override;

        void Update() override;

        void Send(PeerId peer, const void *data, size_t size, ChannelId channel, DeliveryType mode) override;

        std::vector<NetworkEvent> PollEvents() override;

    private:
        ENetHost *m_Host = nullptr;

        PeerId m_NextPeerId = 1;

        std::unordered_map<PeerId, ENetPeer*> m_Peers;

        std::unordered_map<ENetPeer*, PeerId> m_PeerIds;

        std::vector<NetworkEvent> m_Events;
    };
}
