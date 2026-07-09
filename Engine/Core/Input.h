//
// Created by sunvy on 15/12/2025.
//

#pragma once

#include "Event.h"
#include "Keys.h"

#include <array>
#include <bitset>
#include <cstdint>
#include <unordered_map>
#include <vector>

#include <glm/vec2.hpp>

namespace Sunset
{
   using InputAction = std::uint64_t;
   constexpr std::uint8_t MaxGamepads = 16;

   struct ActionValue
   {
      glm::vec2 value = glm::vec2(0.0f);

      bool AsBool() const;
      float AsFloat() const;
      glm::vec2 AsVector2() const;
   };

   template <typename TKey, std::uint16_t nbr>
   struct InputState
   {
      bool IsDown(TKey key) const
      {
         return keys.test(static_cast<std::uint16_t>(key));
      }
      void SetKey(TKey key, bool down)
      {
         keys.set(static_cast<std::uint16_t>(key), down);
      }
   private:
      std::bitset<nbr> keys;
   };

   struct MouseState
   {
      InputState<MouseKey, static_cast<std::uint16_t>(MouseKey::Count)> Buttons;
      glm::vec2 Position = glm::vec2(0.0f);
      glm::vec2 Delta = glm::vec2(0.0f);
      glm::vec2 Scroll = glm::vec2(0.0f);
   };

   struct GamepadState
   {
      bool Connected = false;
      InputState<GamepadButton, static_cast<std::uint16_t>(GamepadButton::Count)> Buttons;
      std::array<float, static_cast<std::uint16_t>(GamepadAxis::Count)> Axes{};
   };

   class InputSystem
   {
   public:
      void Begin();
      void ProcessEvent(const Event::Type& event);

      void SetKey(Key key, bool down);
      void SetMouseButton(MouseKey key, bool down);
      void SetMousePosition(glm::vec2 position);
      void AddMouseScroll(glm::vec2 offset);
      void SetGamepadConnected(std::uint8_t gamepad, bool connected);
      void SetGamepadButton(std::uint8_t gamepad, GamepadButton button, bool down);
      void SetGamepadAxis(std::uint8_t gamepad, GamepadAxis axis, float value);

      bool IsKeyDown(Key key) const;
      bool IsPressed(Key key) const;
      bool IsReleased(Key key) const;

      bool IsMouseButtonDown(MouseKey key) const;
      bool IsMouseButtonPressed(MouseKey key) const;
      bool IsMouseButtonReleased(MouseKey key) const;
      glm::vec2 GetMousePosition() const;
      glm::vec2 GetMouseDelta() const;
      glm::vec2 GetMouseScroll() const;

      bool IsGamepadConnected(std::uint8_t gamepad) const;
      bool IsGamepadButtonDown(std::uint8_t gamepad, GamepadButton button) const;
      bool IsGamepadButtonPressed(std::uint8_t gamepad, GamepadButton button) const;
      bool IsGamepadButtonReleased(std::uint8_t gamepad, GamepadButton button) const;
      float GetGamepadAxis(std::uint8_t gamepad, GamepadAxis axis) const;
   private:
      InputState<Key, 512> Previous;
      InputState<Key, 512> Current;
      MouseState PreviousMouse;
      MouseState CurrentMouse;
      std::array<GamepadState, MaxGamepads> PreviousGamepads;
      std::array<GamepadState, MaxGamepads> CurrentGamepads;
   };

   class InputMapping
   {
   public:
      void Bind(Key key, InputAction action);
      void Bind(MouseKey key, InputAction action);
      void Bind(GamepadButton button, InputAction action, std::uint8_t gamepad = 0);
      void BindAxis(GamepadAxis axis, InputAction action, std::uint8_t gamepad = 0, float scale = 1.0f);

      bool IsActionDown(InputAction action, const InputSystem& input) const;
      bool IsActionPressed(InputAction action, const InputSystem& input) const;
      bool IsActionReleased(InputAction action, const InputSystem& input) const;
      ActionValue GetActionValue(InputAction action, const InputSystem& input) const;
   private:
      struct GamepadButtonBinding
      {
         std::uint8_t gamepad = 0;
         GamepadButton button = GamepadButton::Unknown;
      };

      struct GamepadAxisBinding
      {
         std::uint8_t gamepad = 0;
         GamepadAxis axis = GamepadAxis::Unknown;
         float scale = 1.0f;
      };

      std::unordered_map<InputAction, std::vector<Key>> m_Bindings;
      std::unordered_map<InputAction, std::vector<MouseKey>> m_MouseBindings;
      std::unordered_map<InputAction, std::vector<GamepadButtonBinding>> m_GamepadButtonBindings;
      std::unordered_map<InputAction, std::vector<GamepadAxisBinding>> m_GamepadAxisBindings;
   };
}
