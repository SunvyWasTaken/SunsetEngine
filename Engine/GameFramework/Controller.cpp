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
}
