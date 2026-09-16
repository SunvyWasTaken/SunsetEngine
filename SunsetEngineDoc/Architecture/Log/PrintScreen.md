(Engine/Log/Logger.h)
## Concept

[[PrintScreen]] stocke des messages temporaires a afficher a l'ecran.

```cpp
PRINTSCREEN("FPS {}", fps);
```

Les messages sont gardes dans une liste pour la frame courante.

```cpp
PrintScreen::Get();
PrintScreen::Clear();
```

[[EditorApplication]] les affiche dans une fenetre ImGui en fin de frame.

