(Engine/Utility/Profiling.h)
## Concept

[[Profiling]] est un profiler de scope.

Quand `SS_PROFILING` est actif, les macros creent un objet temporaire.

```cpp
SS_PROFILE_SCOPE("Render part");
SS_PROFILE_FUNCTION();
```

Le constructeur memorise le temps de depart.
Le destructeur calcule la duree et ajoute une ligne dans [[ProfileData]].

## Affichage

[[EditorApplication]] lit `ProfileData::Get()` et affiche les resultats dans une fenetre ImGui.

Les donnees sont nettoyees a la fin de la frame par [[Application]].

