//
// Created by sunvy on 01/06/2026.
//

#pragma once

#include "NetworkType.h"

namespace Sunset
{
    namespace NetworkEvent
    {
        struct PeerConnected
        {
            PeerId Peer;
        };

        struct PeerDisconnected
        {
            PeerId Peer;
        };

        struct PacketReceived
        {
            Packet packet;
        };

        using Type = std::variant<PeerConnected, PeerDisconnected, PacketReceived>;
    }
}
