(Engine/Reflection/ReflectionType.h)
## Concept

[[ReflectionType]] decrit les champs editables d'un objet.

Il est utilise par l'editor pour dessiner des proprietes sans connaitre chaque classe a la main.

```cpp
ReflectionType type;
type.Name = "Transform";
type.Field("Position", &TransformComponent::Position);
type.Field("Scale", &TransformComponent::Scale);
```

Chaque champ contient:

- un nom
- un type
- une fonction qui retourne le pointeur vers la donnee

```cpp
struct ReflectionField
{
	std::string Name;
	ReflectionFieldType Type;
	std::function<void*(void*)> GetPtr;
};
```

## Enums

Les enums sont declarees avec une liste de valeurs.

```cpp
type.EnumField("Projection", &CameraComponent::Projection, {
	{ProjectionType::Perspective, "Perspective"},
	{ProjectionType::Orthographic, "Orthographic"}
});
```

Cela permet a l'editor d'afficher un combo.

