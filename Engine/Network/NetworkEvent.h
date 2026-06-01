//
// Created by sunvy on 01/06/2026.
//

#pragma once

#include "NetworkType.h"

namespace Sunset
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
        PeerId Peer;
        std::vector<std::byte> Data;
    };

    using NetworkEvent = std::variant<PeerConnected, PeerDisconnected, PacketReceived>;
}
