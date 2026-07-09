//
// Created by sunvy on 15/12/2025.
//

#include "Input.h"

#include <GLFW/glfw3.h>

namespace
{

}

namespace Sunset
{
    void InputSystem::Begin()
    {
        Previous = Current;
    }

    void InputSystem::SetKey(Key key, bool down)
    {
        Current.SetKey(key, down);
    }

    bool InputSystem::IsKeyDown(Key key) const
    {
        return Current.IsDown(key);
    }

    bool InputSystem::IsPressed(Key key) const
    {
        return false;
    }

    bool InputSystem::IsReleased(Key key) const
    {
        return false;
    }

    void InputMapping::Bind(Key key, ActionMask action)
    {
        m_Bindings[action].emplace_back(key);
    }

    bool InputMapping::IsActionDown(ActionMask action, const InputSystem &input) const
    {
        auto it = m_Bindings.find(action);
        if (it != m_Bindings.end())
            return false;

        for (const auto& key : it->second)
        {
            if (input.IsKeyDown(key))
                return true;
        }
        return false;
    }
}

