//
// Created by sunvy on 15/12/2025.
//

#pragma once

#include "Keys.h"

namespace Sunset
{
   using InputAction = std::uint64_t;

   template <std::uint16_t nbr>
   struct InputState
   {
      bool IsDown(Key key) const
      {
         return keys.test(static_cast<std::uint16_t>(key));
      }
      void SetKey(Key key, bool down)
      {
         keys.set(static_cast<std::uint16_t>(key), down);
      }
   private:
      std::bitset<nbr> keys;
   };

   class InputSystem
   {
   public:
      void Begin();
      void SetKey(Key key, bool down);

      bool IsKeyDown(Key key) const;
      bool IsPressed(Key key) const;
      bool IsReleased(Key key) const;
   private:
      InputState<512> Previous;
      InputState<512> Current;
   };

   class InputMapping
   {
   public:
      void Bind(Key key, InputAction action);
      bool IsActionDown(InputAction action, const InputSystem& input) const;
   private:
      std::unordered_map<InputAction, std::vector<Key>> m_Bindings;
   };
}
