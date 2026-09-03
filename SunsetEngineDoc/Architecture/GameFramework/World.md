## Concept

Le [[World]] est juste un wrapper pour entt::registry de [EnTT](https://github.com/skypjack/entt).

```cpp
class World
{
	entt::registry m_Registry;
}
```

Il est responsable de crée [[Entity]].

```cpp
Entity World::CreateEntity(const std::string& name);
```

## Réflection

Alors j'ai choisi de ne pas mettre le world directement dans application
parce que je souhaite faire d'autre type d'application a partir de cet base.
donc world etant surtout pour du jeux video il esr a rajoute plus tard dans un layer
comme dans editor layer.
