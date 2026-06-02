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

        m_Transport->PollEvents();
    }

    void NetworkService::Shutdown()
    {
        LOG("Engine", info, "Shutdown Server")
        g_Sunset.reset();
    }

    void NetworkService::Send()
    {
        if (!m_Transport)
            return;
    }

    void NetworkService::Broadcast()
    {
        if (!m_Transport)
            return;
    }
}
