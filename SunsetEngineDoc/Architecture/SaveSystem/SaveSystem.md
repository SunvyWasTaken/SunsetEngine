(Engine/SaveSystem/SaveSystem.h)
## Concept

[[SaveSystem]] est une facade simple autour de [[BinaryArchive]].

```cpp
SaveSystem::Save(path, world);
SaveSystem::Load(path, world);
```

Il ouvre le fichier en binaire, cree l'archive correspondante,
puis appelle la serialization du type.

```cpp
BinaryOutputArchive archive(file);
archive(data);
```

## Dossiers

`CreateFolder` sert a garantir qu'un dossier existe avant d'ecrire dedans.

