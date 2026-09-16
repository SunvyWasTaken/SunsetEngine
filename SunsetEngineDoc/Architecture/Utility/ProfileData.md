(Engine/Utility/Profiling.h)
## Concept

[[ProfileData]] stocke les resultats produits par [[Profiling]].

```cpp
ProfileData::Get();
ProfileData::Free();
```

Les mesures sont ajoutees pendant la frame,
puis affichees par [[EditorApplication]].

[[Application]] nettoie les donnees en fin de frame.

