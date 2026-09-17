(Editor/Panels/ContentBrowserPanel.h)

## Concept

[[ContentBrowserPanel]] affiche et manipule les fichiers dans `CONTENT_PATH`.

Il est rendu dans une fenetre ImGui `Content`.

```cpp
ContentBrowserPanel panel;
panel.SetWorld(world);
panel.OnImGuiRender();
```

## Affichage

Le panel affiche les dossiers et fichiers sous forme de tuiles.

- les dossiers utilisent `SUNSET_EDITOR_LOCAL_RESOURCES "Folder.png"`;
- les fichiers utilisent `SUNSET_EDITOR_LOCAL_RESOURCES "File.png"`;
- le nom est affiche sous l'icone;
- les dossiers sont affiches avant les fichiers;
- les entrees sont triees par nom.

Les icones sont chargees en textures avec `stb_image` et `Texture::Create`.

## Navigation

Le panel garde un chemin courant:

```cpp
std::filesystem::path currentPath = CONTENT_PATH;
```

Un clic gauche sur un dossier navigue dedans.

Le bouton retour revient au dossier racine `CONTENT_PATH`.

## Creation et renommage

Un clic droit dans la fenetre ouvre le menu de creation:

- `Create Folder`

Un clic droit sur un dossier ouvre un menu contextuel:

- `Rename`

Le rename ouvre une popup modale, verifie que le nom n'est pas vide et refuse d'ecraser un fichier ou dossier existant.

## Drag and drop

Les fichiers et dossiers peuvent etre drag depuis leur tuile.

Les dossiers sont des drop targets.

Exemple:

- drag `World.bin`;
- drop sur le dossier `World`;
- le fichier devient `Content/World/World.bin`.

Le deplacement utilise `std::filesystem::rename`.

Protections:

- pas d'ecrasement si un item du meme nom existe deja;
- pas de deplacement d'un dossier dans lui-meme;
- mise a jour de `m_CurrentWorldPath` si le world ouvert est deplace.

## World courant

Le panel garde le chemin du world actuellement ouvert:

```cpp
std::filesystem::path m_CurrentWorldPath;
```

Ce chemin est utilise par [[EditorLayer]] pour `File > Save`.

Quand un fichier `.bin` est ouvert avec succes, le chemin devient le world courant.

## Ouverture par double clic

Un double clic gauche sur un fichier `.bin` charge le world dans le [[World]] courant:

```cpp
if (SaveSystem::Load(file.path(), *m_World))
	m_CurrentWorldPath = file.path();
```

Le [[World]] est injecte avec:

```cpp
panel.SetWorld(world);
```

