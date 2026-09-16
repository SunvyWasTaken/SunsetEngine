(Engine/Utility/Definitions.h)
## Concept

[[Definitions]] contient de petits symboles globaux utilises par l'engine.

```cpp
#define SUNSET_API __declspec(dllexport)
```

Il contient aussi `always_false_v`, utile pour produire des erreurs de compilation dans les templates.

```cpp
template <typename T>
inline constexpr bool always_false_v = false;
```

On le retrouve par exemple dans [[BinaryArchive]] ou [[ReflectionType]]
quand un type n'a pas de specialisation.

