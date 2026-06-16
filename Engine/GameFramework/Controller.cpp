//
// Created by sunvy on 03/06/2026.
//

#include "Controller.h"

#include "Components/TransformComponent.h"
#include "Core/Application.h"
#include "Core/Input.h"

namespace Sunset
{
    Controller::Controller(PeerId peer, std::unique_ptr<IInputSource>& inputSource)
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

        if (auto* comp = m_entity.GetComponent<TransformComponent>())
        {
            constexpr float speed = 100.f;
            if (Check(inputs["Forward"]))
                comp->AddLocation(comp->GetForwardVector() * speed * dt);
            if (Check(inputs["Backward"]))
                comp->AddLocation(-comp->GetForwardVector() * speed * dt);
            if (Check(inputs["Left"]))
                comp->AddLocation(-comp->GetRightVector() * speed * dt);
            if (Check(inputs["Right"]))
                comp->AddLocation(comp->GetRightVector() * speed * dt);
            if (Check(inputs["Up"]))
                comp->AddLocation(glm::vec3{0, 1, 0} * speed * dt);
            if (Check(inputs["Down"]))
                comp->AddLocation(glm::vec3{0, -1, 0} * speed * dt);

            if (!Application::IsHeadless() && dynamic_cast<LocalInputSource*>(m_InputSource.get()) != nullptr)
            {
                glm::vec2 mous = InputRegister::GetMouseDelta();
                if (mous.length() >= 0.001)
                {
                    //mous = glm::normalize(mous);
                    comp->Rotate(comp->GetRightVector(), -mous.y * 0.005f);
                    comp->Rotate({0, 1, 0}, -mous.x * 0.005f);
                }
            }
        }
    }

    PeerId Controller::GetPeerId() const
    {
        return m_peerId;
    }

    void Controller::SetPeerId(PeerId peer)
    {
        m_peerId = peer;
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
