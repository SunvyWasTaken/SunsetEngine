(Editor/Core/EditorApplication.h)
## Concept

[[EditorApplication]] est une [[Application]] specialisee pour l'editor.

Elle ajoute ImGui autour de la boucle de frame.

```cpp
void BeginFrame() override;
void EndFrame() override;
void OnWindowReady() override;
```

`BeginFrame` demarre une frame ImGui.

`EndFrame` dessine les fenetres ImGui, puis appelle la fin de frame de [[Application]].

## GameModule

Au moment ou la window est prete, [[EditorApplication]] charge aussi le module de jeu avec [[GameModuleLoader]].

```cpp
loader.Load(SUNSET_EDITOR_GAME_MODULE_PATH, *this);
```

