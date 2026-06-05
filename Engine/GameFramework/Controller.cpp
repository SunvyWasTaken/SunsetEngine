//
// Created by sunvy on 03/06/2026.
//

#include "Controller.h"
#include "Core/Input.h"

namespace Sunset
{
    Controller::Controller(PeerId peer, std::unique_ptr<IInputSource>&& inputSource)
        : m_peerId(peer)
        , m_InputSource(std::move(inputSource))
    {
    }

    void Controller::Update(float dt)
    {
        SS_PROFILE_FUNCTION();
        // Todo : Pour le moment je vais mettre les déplacement ici.
        InputState inputs = m_InputSource->GetInput();

        auto Check = [&](const Event::Type& action)->bool
        {
            return std::visit(overloads{
                [](const Event::KeyEvent& event)
                {
                    return event.action == Event::Action::Press;
                },
                [](const Event::MouseEvent& mouseEvent)
                {
                    return mouseEvent.action == Event::Action::Press;
                }
            }, action);
        };

        if (Check(inputs["Forward"]))
        {
            if (auto* comp = m_entity.GetComponent<TransformComponent>())
                comp->AddLocation(glm::vec3{100, 0, 0} * dt);
        }
        else if (Check(inputs["Backward"]))
        {
            if (auto* comp = m_entity.GetComponent<TransformComponent>())
                comp->AddLocation(glm::vec3{-100, 0, 0} * dt);
        }
        else if (Check(inputs["Left"]))
        {
            if (auto* comp = m_entity.GetComponent<TransformComponent>())
                comp->AddLocation(glm::vec3{0, 0, -100} * dt);
        }
        else if (Check(inputs["Right"]))
        {
            if (auto* comp = m_entity.GetComponent<TransformComponent>())
                comp->AddLocation(glm::vec3{0, 0, 100} * dt);
        }
    }

    PeerId Controller::GetPeerId() const
    {
        return m_peerId;
    }

    Entity Controller::GetEntity() const
    {
        return m_entity;
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
