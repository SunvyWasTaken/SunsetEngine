(Engine/SaveSystem/BinaryArchive.h)
## Concept

[[BinaryArchive]] est le mecanisme de serialization binaire de l'engine.

Il existe deux archives:

```cpp
BinaryInputArchive input(file);
BinaryOutputArchive output(file);
```

Elles savent lire et ecrire les types arithmetiques, enums, strings,
vectors, arrays et quelques types GLM.

Pour les autres types, il faut definir une fonction `Serialize`.

```cpp
template <typename Archive>
void Serialize(Archive& ar, TransformComponent& component)
{
	ar(component.Position);
	ar(component.Rotation);
	ar(component.Scale);
}
```

## Direction

Le meme `Serialize` peut servir en lecture et en ecriture.

L'archive decide si `ar(value)` lit ou ecrit selon son type.

