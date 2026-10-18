(Editor/Layers/EditorLayer.h)

## Concept

[[EditorLayer]] est le layer principal de l'éditeur. Il possède le [[World]] actif, la hiérarchie et le navigateur de contenu.

Le panneau Viewport est actuellement vide. Il sert de point de départ pour une nouvelle implémentation du rendu de scène.

## Open / Save

`File > Open` utilise [[FileDialog]] pour choisir un fichier `.bin`. `File > Save` écrit dans le fichier courant ou ouvre la popup `Save World As` pour créer un fichier dans `CONTENT_PATH`.

## Play / Stop

Au Play, l'éditeur crée une copie du monde avec `World::Clone()`, démarre les scripts et met ce monde à jour. Au Stop, il revient au monde d'édition original. Les changements faits pendant le Play ne sont pas conservés.
