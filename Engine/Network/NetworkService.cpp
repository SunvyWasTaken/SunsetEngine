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
        m_Transport = std::make_unique<ENetTransport>();
    }

    NetworkService::~NetworkService()
    {
        m_Transport->Shutdown();
    }

    void NetworkService::Init()
    {
        g_Sunset = std::make_unique<NetworkService>();
    }

    void NetworkService::Shutdown()
    {
        g_Sunset.reset();
    }

    NetworkService & NetworkService::Get()
    {
        if (!g_Sunset)
        {
            Init();
            // LOG("Engine", error, "U forgot to call NetworkService::Init() first");
            // DEBUG_BREAK();
        }

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
                [&](NetworkEvent::PeerConnected& e)
                {
                    LOG("Engine", trace, "Peer connected")
                    for (const auto& handler : m_PeerConnectedHandlers)
                        handler(e.Peer);
                },
                [&](NetworkEvent::PeerDisconnected& e)
                {
                    LOG("Engine", trace, "Peer disconnected")
                    for (const auto& handler : m_PeerDisconnectedHandlers)
                        handler(e.Peer);
                },
                [&](NetworkEvent::PacketReceived& e)
                {
                    LOG("Engine", trace, "Packet received")
                    Dispatch(e.packet.peer, e.packet.channel, e.packet.payload);
                }
            }, event);
        }
    }

    void NetworkService::RegisterPeerConnectedHandler(std::function<void(PeerId)> handler)
    {
        m_PeerConnectedHandlers.emplace_back(std::move(handler));
    }

    void NetworkService::RegisterPeerDisconnectedHandler(std::function<void(PeerId)> handler)
    {
        m_PeerDisconnectedHandlers.emplace_back(std::move(handler));
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
