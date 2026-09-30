(Editor/Core/FileDialog.h)

## Concept

[[FileDialog]] encapsule l'ouverture d'une fenetre native du systeme pour choisir un fichier.

Il est utilise par [[EditorLayer]] pour `File > Open` et `File > Import`.

```cpp
auto worldPath = FileDialog::OpenFile("Open World", CONTENT_PATH, {{"World files", "*.bin"}});
auto assetPath = FileDialog::OpenFile("Asset", SUNSET_RESOURCES);
```

Le troisieme argument est une liste facultative de filtres `{libelle, motif}`. Sans filtre, tous les fichiers sont affiches. Avec des filtres, le dialogue propose aussi « All files ».

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
const auto worldPath = FileDialog::OpenFile("Open World", CONTENT_PATH, {{"World files", "*.bin"}});
if (worldPath && SaveSystem::Load(*worldPath, *m_World))
	m_ContentBrowserPanel.SetCurrentWorldPath(*worldPath);
```
