//
// Created by sunvy on 02/06/2026.
//

#pragma once

#include "NetworkType.h"

namespace Sunset
{
    class INetworkTransport;

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

        template <typename T>
        void RegisterHandler(std::function<void(PeerId, const T&)> handler)
        {

        }

    private:
        std::unique_ptr<INetworkTransport> m_Transport;

    };
}
