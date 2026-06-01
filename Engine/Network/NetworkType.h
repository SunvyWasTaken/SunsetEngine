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

    struct EndPoint
    {
        std::string Address;
        uint16_t Port;
    };

    enum class DeliveryType
    {
        Reliable = 0,
        Unreliable = 1,
    };
}
