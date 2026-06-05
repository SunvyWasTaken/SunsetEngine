//
// Created by sunvy on 02/06/2026.
//

#include "NetworkService.h"
#include "INetworkTransport.h"
#include "ENet/ENetTransport.h"

namespace
{
    std::unique_ptr<Sunset::NetworkService> g_Sunset = nullptr;
}

namespace Sunset
{
    NetworkService::NetworkService()
    {
        LOG("Engine", info, "NetworkService::NetworkService")
        m_Transport = std::make_unique<ENetTransport>();
    }

    NetworkService::~NetworkService()
    {
        m_Transport->Shutdown();
    }

    NetworkService & NetworkService::Get()
    {
        if (!g_Sunset)
            g_Sunset = std::make_unique<NetworkService>();

        return *g_Sunset;
    }

    bool NetworkService::Host(uint16_t port, uint32_t maxPlayer)
    {
        if (!m_Transport)
            return false;

        if (!m_Transport->StartServer(port, maxPlayer))
            return false;

        LOG("Engine", info, "Server Start success")

        return true;
    }

    bool NetworkService::Join(const EndPoint &endpoint)
    {
        if (!m_Transport)
            return false;

        if (!m_Transport->Connect(endpoint))
            return false;

        LOG("Engine", info, "Server Connect success")

        return true;
    }

    void NetworkService::Update(float dt)
    {
        if (!m_Transport)
            return;

        auto events = m_Transport->PollEvents();

        for (auto& event : events)
        {
            std::visit(overloads
            {
                [&](NetworkEvent::PeerConnected)
                {
                    LOG("Engine", trace, "Peer connected")
                },
                [&](NetworkEvent::PeerDisconnected)
                {
                    LOG("Engine", trace, "Peer disconnected")
                },
                [&](NetworkEvent::PacketReceived& e)
                {
                    LOG("Engine", trace, "Packet received")
                    Dispatch(e.packet.peer, e.packet.channel, e.packet.payload);
                }
            }, event);
        }
    }

    void NetworkService::Shutdown()
    {
        LOG("Engine", info, "Shutdown Network Service")
        g_Sunset.reset();
    }

    void NetworkService::Send(PeerId peer, ChannelId channel, std::span<const std::byte> payload, DeliveryType mode)
    {
        if (!m_Transport)
            return;

        m_Transport->Send(peer, channel, payload, mode);
        m_Transport->Flush();
    }

    void NetworkService::Broadcast(ChannelId channel, std::span<const std::byte> payload, DeliveryType mode)
    {
        if (!m_Transport)
            return;

        m_Transport->Broadcast(channel, payload, mode);
        m_Transport->Flush();
    }

    void NetworkService::Dispatch(PeerId peer, ChannelId channel, std::span<const std::byte> payload)
    {
        const auto handler = m_Handlers.find(channel);
        if (handler == m_Handlers.end())
        {
            LOG("Engine", warn, "No Network handler registered for channel {}", channel);
            return;
        }

        handler->second(peer, payload);
    }
}
