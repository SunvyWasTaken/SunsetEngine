## Concept

[[Entity]] est juste un wrapper de entt::entity de [EnTT](https://github.com/skypjack/entt).

```cpp
class Entity
{
	entt::entity m_Id;
}
```

Il se charge aussi de crée c'est propre [[Component]].

```cpp
template <typename T, typename ...Args>
requires(std::is_base_of_v<Component, T>)
T& AddComponent(Args&&... args);
```

## How to use

Pour avoir un entity il vous faut un [[World]]
pour crée une nouvelle entity.

```cpp
World world;
Entity entity = world.CreatEntity("EntityName");
```

Ensuite vous pouvez juste call AddComponent pour cree un component.
```cpp
// ...
TransformComponent transform = entity.AddComponent<TransformComponent>();
```

Les entités n'ont pas besoin d'être géré par l'utilisateur.
le moteur se charge automatiquement de leur cycle de vie.
