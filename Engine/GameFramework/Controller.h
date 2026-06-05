//
// Created by sunvy on 03/06/2026.
//

#pragma once
#include "World/Entity.h"

namespace Sunset
{
    class IInputSource;
    using PeerId = uint32_t;
    class Controller
    {
    public:
        Controller(PeerId peer, std::unique_ptr<IInputSource>& inputSource);
        virtual ~Controller() = default;

        Controller(const Controller&) = delete;
        Controller& operator=(const Controller&) = delete;

        Controller(Controller&&) noexcept = default;
        Controller& operator=(Controller&&) noexcept = default;

        void Update(float dt);

        [[nodiscard]] PeerId GetPeerId() const;
        [[nodiscard]] Entity GetEntity() const;

        void Possess(const Entity& entity);
        void UnPossess();

    private:
        PeerId m_peerId{};
        Entity m_entity;
        std::unique_ptr<IInputSource> m_InputSource;
    };
}
