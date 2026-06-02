//
// Created by sunvy on 01/06/2026.
//

#include "ENetTransport.h"

#include <enet/enet.h>

namespace Sunset
{
    struct ENetTransport::Impl
    {
        explicit operator bool() const
        {
            return client != nullptr && peer != nullptr;
        }

        ENetHost* client = nullptr;
        ENetPeer* peer = nullptr;
    };


    ENetTransport::ENetTransport()
    {
        LOG("Engine", info, "ENetTransport Create");
        m_Impl = std::make_unique<Impl>();
        if (enet_initialize() != 0)
        {
            LOG("Engine", error, "Failed to initialize");
            return;
        }
    }

    ENetTransport::~ENetTransport()
    {
        LOG("Engine", info, "ENetTransport Destroy");
        enet_deinitialize();
    }

    bool ENetTransport::StartServer(uint16_t port, uint32_t maxClients)
    {
        m_Impl->client = enet_host_create(NULL, maxClients, maxClients, 0, 0);
        if (m_Impl->client == nullptr)
        {
            LOG("Engine", error, "Failed to create ENetHost");
            return false;
        }
        return true;
    }

    bool ENetTransport::Connect(const EndPoint& endpoint)
    {
        ENetAddress address;

        enet_address_set_host(&address, endpoint.Address.c_str());
        address.port = endpoint.Port;
        m_Impl->peer = enet_host_connect(m_Impl->client, &address, 0, 0);
        if (m_Impl->peer == nullptr)
        {
            LOG("Engine", error, "Failed to connect to ENetHost");
            return false;
        }
        return true;
    }

    void ENetTransport::Disconnect(PeerId peer)
    {
    }

    void ENetTransport::Send(PeerId peer, ChannelId channel, std::span<const std::byte> payload, DeliveryType mode)
    {
    }

    void ENetTransport::Broadcast(ChannelId channel, std::span<const std::byte> payload, DeliveryType mode)
    {
    }

    std::vector<NetworkEvent::Type> ENetTransport::PollEvents()
    {
        if (!*m_Impl)
            return {};

        ENetEvent event;
        if (enet_host_service(m_Impl->client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
        {
            LOG("Engine", info, "Connection to 127.0.0.1:7777 succeed");
        }
        return {};
    }

    void ENetTransport::Flush()
    {
    }

    void ENetTransport::Shutdown()
    {
    }
}
