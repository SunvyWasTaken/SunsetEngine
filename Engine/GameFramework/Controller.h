//
// Created by sunvy on 03/06/2026.
//

#pragma once
#include "World/Entity.h"

namespace Sunset
{
    using PeerId = uint32_t;
    class Controller
    {
    public:
        explicit Controller(PeerId peer);
        virtual ~Controller() = default;

        [[nodiscard]] PeerId GetPeerId() const;

        void Possess(const Entity& entity);
        void UnPossess();

    private:
        PeerId m_peerId;
        Entity m_entity;
    };
}
