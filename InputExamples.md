# Input examples

These examples show how to use `InputComponent` with keyboard, mouse, and gamepad bindings.

## Action ids

`InputAction` is currently a `std::uint64_t`, so projects can define their own action ids:

```cpp
enum PlayerAction : Sunset::InputAction
{
    MoveX = 1,
    Jump = 2,
    Fire = 3,
    Pause = 4,
};
```

## Add bindings from gameplay code

```cpp
auto& input = entity.AddComponent<Sunset::InputComponent>();

// Keyboard
input.BindAction(Sunset::Key::Space, PlayerAction::Jump);
input.BindAction(Sunset::Key::Escape, PlayerAction::Pause);

// Mouse
input.BindAction(Sunset::MouseKey::Left, PlayerAction::Fire);

// Gamepad button
input.BindAction(Sunset::GamepadButton::South, PlayerAction::Jump);

// Gamepad axis. Use scale to invert or remap direction.
input.BindAxis(Sunset::GamepadAxis::LeftX, PlayerAction::MoveX, 0, 1.0f);
```

## Read actions in a script

```cpp
void PlayerScript::OnUpdate(float dt)
{
    auto& input = GetComponent<Sunset::InputComponent>();

    if (input.IsActionPressed(PlayerAction::Jump))
    {
        Jump();
    }

    if (input.IsActionDown(PlayerAction::Fire))
    {
        FireWeapon();
    }

    const float moveX = input.GetActionValue(PlayerAction::MoveX).AsFloat();
    MoveRight(moveX * dt);
}
```

## Read raw mouse state

```cpp
const Sunset::InputSystem& rawInput = input.GetInputSystem();

if (rawInput.IsMouseButtonPressed(Sunset::MouseKey::Right))
{
    StartCameraOrbit();
}

const glm::vec2 mouseDelta = rawInput.GetMouseDelta();
const glm::vec2 scroll = rawInput.GetMouseScroll();
```

## Read raw gamepad state

```cpp
const Sunset::InputSystem& rawInput = input.GetInputSystem();

if (rawInput.IsGamepadConnected(0))
{
    const float lookX = rawInput.GetGamepadAxis(0, Sunset::GamepadAxis::RightX);
    const bool pressed = rawInput.IsGamepadButtonPressed(0, Sunset::GamepadButton::South);
}
```

## Configure bindings in the editor

1. Select an entity in the hierarchy.
2. In `Properties`, click `Add Input Component` if the entity does not have one yet.
3. Open the `Input` section.
4. Add a keyboard, mouse, gamepad button, or gamepad axis binding.
5. Set the numeric `Action` id to the same value used by your gameplay code.
