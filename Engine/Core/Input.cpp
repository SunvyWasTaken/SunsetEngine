//
// Created by sunvy on 15/12/2025.
//

#include "Input.h"

namespace
{

}

namespace Sunset
{
    void InputSystem::Begin()
    {
        Previous = Current;
    }

    void InputSystem::SetKey(const Key key, const bool down)
    {
        Current.SetKey(key, down);
    }

    bool InputSystem::IsKeyDown(const Key key) const
    {
        return Current.IsDown(key);
    }

    bool InputSystem::IsPressed(const Key key) const
    {
        return Current.IsDown(key) && !Previous.IsDown(key);;
    }

    bool InputSystem::IsReleased(const Key key) const
    {
        return !Current.IsDown(key) && Previous.IsDown(key);
    }

    void InputMapping::Bind(Key key, const InputAction action)
    {
        m_Bindings[action].emplace_back(key);
    }

    bool InputMapping::IsActionDown(const InputAction action, const InputSystem &input) const
    {
        const auto it = m_Bindings.find(action);
        if (it == m_Bindings.end())
            return false;

        for (const auto& key : it->second)
        {
            if (input.IsKeyDown(key))
                return true;
        }
        return false;
    }
}

