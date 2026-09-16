(Engine/Utility/UtilityFunction.h)
## Concept

[[UtilityFunction]] regroupe des helpers generiques.

```cpp
UtilityFunction::DoesFileExist(path);
UtilityFunction::OpenTextFile(path);
UtilityFunction::GetRandInRange(0, 10);
```

Ces fonctions ne portent pas d'etat.

Elles servent aux endroits ou un petit service commun suffit sans creer un systeme complet.

