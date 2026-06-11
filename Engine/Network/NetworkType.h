//
// Created by sunvy on 01/06/2026.
//

#pragma once

#include <cstdint>
#include <string>

namespace Sunset
{
    using PeerId = uint32_t;
    using ChannelId = uint8_t;

    /// Represents a raw packet after it has been extracted from the transport layer.
    /// The service uses the channel to dispatch the payload to the matching message handler.
    struct Packet
    {
        PeerId peer = 0;
        ChannelId channel = 0;
        std::vector<std::byte> payload;
    };

    /// Stores the network address and port used when connecting to a remote host.
    struct EndPoint
    {
        std::string Address = "127.0.0.1";
        uint16_t Port = 7777;
    };

    /// Defines the delivery guarantee requested for outgoing packets.
    enum class DeliveryType
    {
        Reliable = 0,
        Unreliable = 1,
    };
}
