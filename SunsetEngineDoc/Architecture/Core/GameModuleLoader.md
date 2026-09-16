(Engine/Core/GameModuleLoader.h)
## Concept

[[GameModuleLoader]] charge une librairie dynamique contenant un [[GameModule]].

Il recupere les fonctions exportees par `CREATE_MODULE`, instancie le module,
puis appelle `Load(Application&)`.

```cpp
GameModuleLoader loader;
loader.Load("Game.dll", app);
```

Au dechargement, il appelle `Unload`, detruit le module, puis libere le handle natif.

## Responsabilite

[[GameModuleLoader]] ne connait pas le contenu du jeu.

Il gere seulement la frontiere entre l'engine et le module charge.

