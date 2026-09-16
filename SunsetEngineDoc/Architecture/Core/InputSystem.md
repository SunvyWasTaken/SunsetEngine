(Engine/Core/Input.h)
## Concept

[[InputSystem]] stocke l'etat brut des inputs.

Il garde l'etat precedent et courant pour detecter:

- down
- pressed
- released

```cpp
input.BeginFrame();
input.ProcessEvent(event);
```

Il gere le clavier, la souris et les gamepads.

```cpp
input.IsKeyDown(Key::Space);
input.GetMouseDelta();
input.GetGamepadAxis(0, GamepadAxis::LeftX);
```

## Mapping

`InputMapping` transforme ces inputs bruts en actions gameplay.

```cpp
mapping.Bind(Key::W, MoveForward);
mapping.IsActionDown(MoveForward, input);
```

[[InputComponent]] combine un [[InputSystem]] et un mapping par entity.

