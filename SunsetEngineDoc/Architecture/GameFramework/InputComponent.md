(Engine/GameFramework/Components/InputComponent.h)
## Concept

[[InputComponent]] donne des bindings d'input a une [[Entity]].

Il possede un [[InputSystem]], un `InputMapping` et une liste de bindings serialisables.

```cpp
component.BindAction(Key::Space, JumpAction);
component.BindAxis(GamepadAxis::LeftX, MoveXAction);
```

Chaque frame, `BeginFrame` reset les etats transitoires.

Les [[Event]] sont ensuite transmis au component avec `OnEvent`.

```cpp
input.BeginFrame();
input.OnEvent(event);
```

## Lecture

Le gameplay lit les actions avec:

```cpp
input.IsActionDown(action);
input.IsActionPressed(action);
input.GetActionValue(action);
```

