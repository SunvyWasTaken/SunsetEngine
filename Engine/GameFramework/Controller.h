//
// Created by sunvy on 03/06/2026.
//

#pragma once

namespace Sunset
{
    using PeerId = uint32_t;
    class Controller
    {
    public:
        explicit Controller(PeerId peer);
        virtual ~Controller() = default;

        [[nodiscard]] PeerId GetPeerId() const;

    private:
        PeerId m_peerId;
    };
}
