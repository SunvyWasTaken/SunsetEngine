# Editor Roadmap

voir si je peux pas le rendre compatible avec un llm
Objectif: rendre l'editeur capable de produire un jeu complet de A a Z, depuis la creation d'une scene jusqu'a l'export jouable.

## 1. Mode Play / Stop

- [x] Ajouter un bouton Play / Stop dans l'editeur.
- [x] Separer le monde d'edition du monde runtime.
- [x] Au Play, cloner le World courant dans un RuntimeWorld.
- [x] Au Stop, detruire le RuntimeWorld et revenir au World d'edition intact.
- [x] Eviter que les scripts modifies pendant le Play changent definitivement la scene.
- [x] Lancer `OnBeginPlay`, `OnUpdate` et `OnEndPlay` des scripts runtime.
- [ ] Ajouter un indicateur visuel clair du mode Play.
- [ ] Bloquer ou filtrer les actions editor dangereuses pendant le Play.

## 2. Gestion de scenes

- [ ] Introduire un vrai concept de scene/world asset.
- [x] Supporter Open Scene, Save et Save As basiques.
- [ ] Supporter New Scene proprement.
- [x] Stocker le chemin de la scene courante.
- Afficher un etat dirty quand la scene a ete modifiee.
- Demander confirmation avant de fermer ou charger une autre scene si des modifications ne sont pas sauvegardees.

## 3. Asset pipeline

- Ajouter des imports propres pour textures, shaders, materials, meshes et audio.
- Generer un fichier metadata par asset, par exemple `.meta`.
- Donner un UUID stable a chaque asset.
- Utiliser les UUID pour les references au lieu de chemins directs.
- Rafraichir le Content Browser quand les assets changent sur disque.

## 4. Prefabs

- Sauvegarder une entite et ses composants comme prefab.
- Instancier un prefab dans une scene.
- Garder un lien optionnel entre une instance et son prefab source.
- Supporter apply/revert overrides plus tard.

## 5. Undo / Redo

- Ajouter une stack de commandes editor.
- Couvrir au minimum:
  - deplacement avec le gizmo
  - modification dans l'inspector
  - creation et suppression d'entite
  - ajout et suppression de composant
  - rename et drag/drop dans le Content Browser
- Grouper les manipulations continues du gizmo en une seule action undo.

## 6. Scripting utilisable dans l'editeur

- Ajouter une interface pour ajouter un NativeScriptComponent.
- Lister les scripts disponibles depuis le module de jeu.
- Exposer les proprietes editables des scripts.
- Recharger le module de jeu proprement.
- Afficher les erreurs de compilation ou de chargement dans une console editor.

## 7. Picking robuste dans le viewport

- [x] Ajouter une selection viewport approximative pour les sprites.
- [ ] Remplacer le picking approximatif par un framebuffer d'ID.
- [ ] Rendre chaque entite selectionnable avec un identifiant unique.
- [ ] Lire le pixel sous la souris au clic.
- [ ] Supporter sprites, meshes 3D et objets superposes.

## 8. Inspector complet

- Ajouter/supprimer des composants depuis l'inspector.
- Copier/coller un composant.
- Reset un composant.
- Editer les rotations sous forme Euler, meme si le moteur stocke un quaternion.
- Drag/drop des assets dans les champs de composants.

## 9. Material editor

- Creer un asset Material.
- Editer shader, textures, couleurs et uniforms.
- Afficher une preview simple.
- Permettre le drag/drop d'un material sur une entite.

## 10. Build / Export

- Ajouter une commande de build standalone.
- Choisir la scene de demarrage.
- Copier les assets necessaires.
- Copier le runtime et le module de jeu.
- Produire une sortie Debug/Release propre.

## 11. Grid et snapping

- Afficher une grid dans le viewport.
- Ajouter snap position, rotation et scale.
- Integrer le snap avec [[ImGuizmo]].
- Rendre les valeurs configurables dans l'editeur.

## 12. Camera editor

- Ajouter orbit, pan et zoom dans le viewport.
- Ajouter focus selected entity.
- Ajouter une vitesse de camera configurable.
- Eviter les conflits entre navigation camera et manipulation gizmo.

## 13. Console et logs

- Ajouter une fenetre Console.
- Afficher les logs engine, editor et game.
- Ajouter filtres par niveau: trace, info, warning, error.
- Afficher les erreurs de load/save et de scripts.

## 14. Project settings

- Ajouter un fichier de configuration projet.
- Stocker nom du jeu, scene de demarrage, resolution et options runtime.
- Stocker les paths d'assets.
- Ajouter une UI Project Settings dans l'editeur.

## 15. Input editor

- Editer les actions input depuis l'editeur.
- Associer clavier, souris et gamepad aux actions.
- Sauvegarder l'input map dans un asset ou dans les project settings.
- Utiliser cette map dans le runtime.

## Ordre recommande

1. Dirty flag et confirmations de scene.
2. Undo / Redo pour transforms et creation/suppression d'entites.
3. Scene asset propre avec New Scene.
4. Add/Remove component dans l'inspector.
5. Asset references avec UUID et metadata.
6. Picking par framebuffer d'ID.
7. Build standalone.

Le mode Play / Stop de base existe maintenant. La priorite suivante est de fiabiliser l'edition autour de la scene: dirty flag, confirmations, undo/redo et workflow scene asset.
