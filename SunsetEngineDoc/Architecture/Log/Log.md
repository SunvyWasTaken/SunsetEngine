(Engine/Log/Logger.h)
## Concept

[[Log]] centralise les loggers de l'engine.

Il est initialise et ferme par [[Application]].

```cpp
Log::Init();
INITLOG("Engine");
LOG("Engine", info, "App Create");
```

En release, les macros peuvent etre desactivees.

```cpp
#define LOG(name, level, txt, ...)
```

## PrintScreen

[[PrintScreen]] stocke des lignes temporaires a afficher dans l'editor.

```cpp
PRINTSCREEN("FPS {}", fps);
```

[[EditorApplication]] lit ces lignes en fin de frame et les affiche dans une fenetre ImGui.

