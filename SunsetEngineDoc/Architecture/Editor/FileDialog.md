(Editor/Core/FileDialog.h)

## Concept

[[FileDialog]] encapsule l'ouverture d'une fenetre native du systeme pour choisir un fichier.

Il est utilise par [[EditorLayer]] pour `File > Open`.

```cpp
auto path = FileDialog::OpenFile("Open World", CONTENT_PATH, "bin");
```

La fonction retourne:

- `std::optional<std::filesystem::path>` avec le fichier choisi;
- `std::nullopt` si l'utilisateur annule ou si aucune implementation n'est disponible.

## Linux

Sous Linux, l'implementation essaie:

1. `zenity`
2. `kdialog`

Si aucun des deux n'est disponible, aucun fichier n'est retourne.

## Windows

Sous Windows, l'implementation utilise l'API native `GetOpenFileNameA`.

## Usage actuel

Dans [[EditorLayer]], l'ouverture d'un world fait:

```cpp
const auto worldPath = FileDialog::OpenFile("Open World", CONTENT_PATH, "bin");
if (worldPath && SaveSystem::Load(*worldPath, *m_World))
	m_ContentBrowserPanel.SetCurrentWorldPath(*worldPath);
```

