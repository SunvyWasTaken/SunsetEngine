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

    struct Packet
    {
        PeerId peer = 0;
        ChannelId channel = 0;
        std::vector<std::byte> payload;
    };

    struct EndPoint
    {
        std::string Address = "127.0.0.1";
        uint16_t Port = 7777;
    };

    enum class DeliveryType
    {
        Reliable = 0,
        Unreliable = 1,
    };
}
