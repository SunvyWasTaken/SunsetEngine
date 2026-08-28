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
