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
