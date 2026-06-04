//
// Created by sunvy on 03/06/2026.
//

#include "Controller.h"

namespace Sunset
{
    Controller::Controller(PeerId peer)
       : m_peerId(peer)
    {
    }

    PeerId Controller::GetPeerId() const
    {
        return m_peerId;
    }

    void Controller::Possess(const Entity &entity)
    {
        m_entity = entity;
    }

    void Controller::UnPossess()
    {
        m_entity = {};
    }
}
