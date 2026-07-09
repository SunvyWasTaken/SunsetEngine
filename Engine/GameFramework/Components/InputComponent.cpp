//
// Created by sunvy on 09/07/2026.
//

#include "InputComponent.h"

namespace Sunset
{
    void InputComponent::BeginFrame()
    {
        inputSystem.Begin();
    }

    void InputComponent::OnEvent(const Event::Type &event)
    {
        if (std::holds_alternative<Event::Keyboard>(event))
        {
            const auto&[key, Press] = std::get<Event::Keyboard>(event);
            inputSystem.SetKey(key, Press);
        }
    }

    bool InputComponent::IsActionDown(const InputAction action) const
    {
        return mapping.IsActionDown(action, inputSystem);
    }
} // Sunset