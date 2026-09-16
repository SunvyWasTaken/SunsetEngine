(Engine/GameFramework/Components/NativeScriptComponent.h)
## Concept

[[NativeScriptComponent]] attache du code C++ a une [[Entity]].

Il cree des instances de [[ScriptEntity]] via `Bind`.

```cpp
entity.AddComponent<NativeScriptComponent>()
	.Bind<PlayerController>();
```

Au demarrage, le component instancie les scripts et leur donne acces a l'entity.

```cpp
script->OnBeginPlay();
script->OnUpdate(dt);
script->OnEndPlay();
```

Le [[NativeScriptingSystem]] est responsable d'appeler les scripts pendant l'update du [[World]].

