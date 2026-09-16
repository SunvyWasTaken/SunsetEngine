(Engine/GameFramework/System/IWorldSystem.h)
## Concept

[[IWorldSystem]] est la base des systemes qui travaillent sur un [[World]].

```cpp
class IWorldSystem
{
public:
	virtual void Update(float dt) {};
protected:
	World* m_World;
};
```

Un systeme est ajoute au [[World]] avec:

```cpp
world.AddSystem<NativeScriptingSystem>();
```

Puis [[World]] appelle `Update` sur chaque systeme.

