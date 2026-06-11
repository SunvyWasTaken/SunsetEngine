//
// Created by sunvy on 01/06/2026.
//

#pragma once

#include "NetworkType.h"

namespace Sunset
{
    namespace NetworkEvent
    {
        /// Emitted when the transport accepts or completes a connection with a peer.
        struct PeerConnected
        {
            PeerId Peer;
        };

        /// Emitted when a known peer disconnects from the current host.
        struct PeerDisconnected
        {
            PeerId Peer;
        };

        /// Emitted when a packet has been received and copied out of the transport backend.
        struct PacketReceived
        {
            Packet packet;
        };

        /// Variant used by transports to return every event type through a single queue.
        using Type = std::variant<PeerConnected, PeerDisconnected, PacketReceived>;
    }
}
