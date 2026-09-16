(Engine/GameFramework/World/ScriptEntity.h)
## Concept

[[ScriptEntity]] est la classe de base pour les scripts natifs.

Un script surcharge les callbacks de gameplay.

```cpp
class PlayerController : public ScriptEntity
{
	void OnBeginPlay() override;
	void OnUpdate(float dt) override;
	void OnEndPlay() override;
};
```

Il peut acceder a ses components.

```cpp
auto* transform = GetComponent<TransformComponent>();
```

Et il peut recuperer le [[World]] auquel son [[Entity]] appartient.

