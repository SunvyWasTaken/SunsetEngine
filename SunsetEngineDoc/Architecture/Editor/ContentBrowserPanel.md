(Editor/Panels/ContentBrowserPanel.h)
## Concept

[[ContentBrowserPanel]] affiche le dossier de contenu de l'editor.

```cpp
ContentBrowserPanel panel;
panel.OnImGuiRender();
```

Il parcourt le dossier `CONTENT_PATH`.

Les dossiers sont affiches comme des boutons.
Les fichiers sont affiches comme du texte.

```cpp
for (auto& file : std::filesystem::directory_iterator(currentPath))
{
	// directory -> button
	// file -> text
}
```

Le panel garde un chemin courant pour naviguer dans les sous-dossiers.

