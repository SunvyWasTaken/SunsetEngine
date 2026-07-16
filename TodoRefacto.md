# Tableau des tâches de stabilisation / extensibilité

---

### Légende :

|    Niveau     | Signification                                  |
|:-------------:|:-----------------------------------------------|
| Importance 5  | Bloquant ou très structurant pour l’avenir     |
| Importance 4  | Très utile avant d’ajouter de grosses features |
| Importance 3  | Important mais pas urgent immédiatement        |
| Complexité S  | Petit chantier                                 |
| Complexité M  | Moyen chantier                                 |
| Complexité L  | Gros refactor                                  |
| Complexité XL | Architecture lourde / long terme               |

---

# Roadmap principale

|  #   | Tâche                                                               | Complexité  | Importance  | Pourquoi c’est important                                                                                                                                                                                               | Zone concernée                          | Moment conseillé                  | Activity |
|:----:|:--------------------------------------------------------------------|:-----------:|:-----------:|:-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|:----------------------------------------|:----------------------------------|:--------:|
|  1   | Corriger les petits bugs bloquants connus                           |      S      |      5      | Avant d’étendre le moteur, il faut fiabiliser les bases : transform non utilisé au rendu, inspector avec casts suspects, `SetScale` vide, etc.                                                                         | `World`, `TransformComponent`, `Editor` | Maintenant                        |   Todo   |
|  2   | Passer le transform ECS au renderer correctement                    |     S/M     |      5      | Actuellement `World::Update` récupère `TransformComponent` et `MeshComponent`, mais appelle `RenderCommande::Submit(mesh.m_mesh)` sans utiliser `transform`.                                                           | `World`, `RenderCommande`               | Maintenant                        |   Todo   |
|  3   | Séparer `World::Update` en systèmes                                 |      L      |      5      | `World::Update` gère scripts, transforms, input, caméra et rendu dans une seule fonction.  À terme, il faut `ScriptSystem`, `TransformSystem`, `CameraSystem`, `RenderSubmissionSystem`, etc.                          | `World`                                 | Maintenant / bientôt              |   Done   |
|  4   | Créer une vraie frontière entre Render API et backend OpenGL        |    L/XL     |      5      | Pour Vulkan plus tard, il faut éviter que tout le moteur dépende directement de `gl*`. `RenderCommande.cpp` contient actuellement conversion d’états vers OpenGL et appels directs `glEnable`, `glDrawElements`, etc.  | `Render/`                               | Avant Vulkan                      |   Todo   |
|  5   | Renommer / restructurer `RenderCommande` en vrai renderer/pipeline  |     M/L     |      5      | `RenderCommande` est actuellement une façade statique avec état global.  Pour l’editor viewport, les passes, Vulkan et particules, il faut une structure moins globale.                                                | `Render/RenderCommande.*`               | Bientôt                           |   Todo   |
|  6   | Créer un `RenderContext` ou `GraphicsDevice`                        |      L      |      5      | C’est la base pour supporter OpenGL maintenant et Vulkan plus tard. Exemple : `IGraphicsDevice`, `OpenGLGraphicsDevice`, puis plus tard `VulkanGraphicsDevice`.                                                        | `Render/Backend`                        | Avant Vulkan                      |   Todo   |
|  7   | Créer des handles/ressources GPU abstraites                         |      L      |      5      | `Shader`, `Texture`, `VertexArray`, `Buffer` exposent ou stockent des IDs OpenGL-like en `uint32_t`.  Pour Vulkan, il faut des abstractions backend-agnostic.                                                          | `Render/Resources`                      | Avant Vulkan                      |   Todo   |
|  8   | Séparer Window/Input platform du renderer                           |     M/L     |      4      | `Render.cpp` mélange création GLFW, callbacks input, ImGui, OpenGL, gamepads.  Pour un moteur propre, la fenêtre/platform devrait être séparée du backend de rendu.                                                    | `Render`, `Core`, `Platform`            | Bientôt                           |   Todo   |
|  9   | Créer une architecture de render passes                             |      L      |      5      | Pour particules, post-process, shadow maps, editor viewport, framebuffer, debug rendering, il faut des passes claires : geometry pass, transparent pass, overlay pass, particle pass, UI pass.                         | `Render/Pipeline`                       | Avant grosses features graphiques |   Todo   |
|  10  | Séparer opaque / transparent / overlay / debug en queues explicites |      M      |      4      | Il y a déjà un tri opaque/transparent/overlay dans `SortDrawCommands`.  C’est une bonne base, mais il faudrait des render queues propres.                                                                              | `RenderCommande`                        | Bientôt                           |   Todo   |
|  11  | Créer un système de particules côté ECS                             |     M/L     |      4      | Avant de faire le rendu particules, il faut un `ParticleEmitterComponent`, simulation CPU/GPU, lifetime, spawn rate, material, texture, bounds.                                                                        | `GameFramework/Components`, `Render`    | Après render queues               |   Todo   |
|  12  | Créer un renderer de particules séparé                              |      L      |      4      | Les particules ne doivent pas être des meshes classiques. Il faut batching/instancing, billboards, tri transparent, éventuellement compute plus tard.                                                                  | `Render/Particles`                      | Après render pipeline             |   Todo   |
|  13  | Stabiliser `Material` et uniformes                                  |      M      |      4      | `Material` stocke des uniformes en `std::variant` et possède directement shader/textures publics.  C’est pratique mais à structurer avant PBR, particules, Vulkan.                                                     | `Render/Material`                       | Bientôt                           |   Todo   |
|  14  | Créer une notion d’AssetHandle / ResourceManager                    |      L      |      5      | `Drawable` possède directement `shared_ptr<Mesh>` et `shared_ptr<Material>`.  Pour scènes, editor, hot reload assets, il faudra des handles persistants.                                                               | `Assets`, `Render`                      | Bientôt                           |   Todo   |
|  15  | Éviter les ressources publiques mutables dans les classes render    |      M      |      4      | `Drawable`, `Material`, `Mesh` exposent beaucoup de membres publics.  Ça rend l’évolution difficile.                                                                                                                   | `Render/*`                              | Progressif                        |   Todo   |
|  16  | Créer `Public/Private` ou `Include/Source`                          |      M      |      5      | Aujourd’hui `Engine/` entier est include public.  Pour stabiliser l’API et préparer une dynamic lib, il faut séparer API publique et détails internes.                                                                 | Repo / CMake                            | Bientôt                           |   Todo   |
|  17  | Créer une macro d’export `SUNSET_API`                               |     S/M     |      3      | Pas urgent si tu restes statique, mais utile pour préparer dynamic lib plus tard.                                                                                                                                      | Core headers                            | Plus tard / progressif            |   Todo   |
|  18  | Aligner CMake / README / standard C++                               |      S      |      4      | Le root demande CMake 3.28, `Engine` demande CMake 4.0.  Ce genre d’incohérence doit disparaître.                                                                                                                      | CMake / docs                            | Maintenant                        |   Done   |
|  19  | Enlever le link direct editor → `SunsetCraft`                       |      M      |      4      | L’editor linke directement `SunsetCraft`.  Pour un editor générique, il faut séparer moteur/editor/projet.                                                                                                             | `Editor/CMakeLists.txt`                 | Bientôt                           |   Todo   |
|  20  | Créer un format de scène sérialisable                               |      L      |      5      | Pour un moteur Unity-like, il faut sauvegarder entités/composants. Sans ça, l’editor restera un outil de debug.                                                                                                        | `SaveSystem`, `World`, `Reflection`     | Priorité haute                    |   Todo   |
|  21  | Étendre la réflexion composants                                     |      L      |      5      | Le système actuel de `ReflectionType` est prometteur mais limité.  Il doit devenir la base de l’inspector + serialization.                                                                                             | `Reflection`, `Components`              | Priorité haute                    |   Todo   |
|  22  | Rendre l’inspector data-driven                                      |      L      |      5      | Aujourd’hui `WorldHierarchyPanel::DrawComponents` connaît manuellement certains composants.  Pour ajouter des composants facilement, il faut un registre de composants.                                                | `Editor/Panels`                         | Bientôt                           |   Todo   |
|  23  | Créer un registre de composants                                     |      L      |      5      | Nécessaire pour ajouter/supprimer des composants depuis l’editor, sérialiser, afficher les noms, créer les composants par type.                                                                                        | `GameFramework`, `Reflection`, `Editor` | Bientôt                           |   Todo   |
|  24  | Créer des UUID persistants pour entités/assets                      |     M/L     |      5      | Sans IDs persistants, scènes, prefabs, références assets et editor stable seront difficiles.                                                                                                                           | `World`, `Assets`                       | Avant scènes avancées             |   Todo   |
|  25  | Créer un Asset Browser minimal                                      |     M/L     |      3      | Pas bloquant pour le renderer, mais très important pour l’expérience Unity-like.                                                                                                                                       | `Editor`, `Assets`                      | Après serialization               |   Todo   |
|  26  | Créer une vraie viewport editor avec framebuffer dédié              |     M/L     |      5      | `FrameBuffer` existe déjà et est plutôt propre.  Il faut l’utiliser comme base pour une viewport editor au lieu de rendre seulement dans la window globale.                                                            | `Editor`, `Render`                      | Bientôt                           |   Todo   |
|  27  | Séparer UI engine / ImGui editor / runtime UI                       |      M      |      3      | `RenderCommande::EndFrame` rend SRmGUI puis ImGui dans le même flux.  Ça marche, mais il faudra clarifier runtime UI vs editor UI.                                                                                     | `Render`, `Editor`, `Thirdparty/SRmGUI` | Moyen terme                       |   Todo   |
|  28  | Créer un système de debug draw                                      |      M      |      3      | Utile pour physics, lights, cameras, particles bounds, navmesh. À séparer du renderer principal.                                                                                                                       | `Render/Debug `                         | Moyen terme                       |   Todo   |
|  29  | Créer des tests simples / sandbox samples                           |      M      |      4      | Pour stabiliser, il faut des scènes de test : mesh, texture, alpha, framebuffer, input, particles, editor.                                                                                                             | `Sandbox`, `Tests`                      | Progressif                        |   Todo   |
|  30  | Documenter les conventions engine                                   |     S/M     |      4      | Noms, ownership, public/private, systèmes, render pipeline, ajout de composant. Ça évitera que le code redevienne brouillon.                                                                                           | Docs                                    | Maintenant/progressif             |   Todo   |

---

## Focus spécial : dossier Render/

Tu as raison : Render/ est probablement le chantier le plus structurant si tu veux Vulkan et particules plus tard.

Actuellement, il contient tout mélangé :
```txt
Render/
    BufferObject/
    Meshes/
    Camera
    Drawable
    FrameBuffer
    Image
    Material
    Render
    RenderCommande
    Shader
    Texture
```

La liste actuelle des fichiers confirme que  `Render/` contient à la fois les ressources GPU, les meshes, le renderer, les framebuffers, le chargement image, les shaders, et la commande de rendu.

### Structure visais à moyen terme
```txt
Engine/Render/
    ├─ Core/
    │   ├─ Renderer.h
    │   ├─ RenderScene.h
    │   ├─ RenderQueue.h
    │   ├─ RenderCommand.h
    │   ├─ RenderState.h
    │   └─ RenderPass.h
    │ 
    ├─ Backend/
    │   ├─ GraphicsDevice.h
    │   ├─ SwapChain.h
    │   ├─ CommandBuffer.h
    │   ├─ Buffer.h
    │   ├─ Texture.h
    │   ├─ Shader.h
    │   └─ Pipeline.h
    │ 
    ├─ Backend/OpenGL/
    │   ├─ OpenGLGraphicsDevice.cpp
    │   ├─ OpenGLBuffer.cpp
    │   ├─ OpenGLTexture.cpp
    │   ├─ OpenGLShader.cpp
    │   └─ OpenGLPipeline.cpp
    │ 
    ├─ Resources/
    │   ├─ Mesh.h
    │   ├─ Material.h
    │   ├─ TextureAsset.h
    │   └─ ShaderAsset.h
    │ 
    ├─ Pipeline/
    │   ├─ ForwardRenderer.h
    │   ├─ GeometryPass.h
    │   ├─ TransparentPass.h
    │   ├─ OverlayPass.h
    │   ├─ ParticlePass.h
    │   └─ UIPass.h
    │ 
    ├─ Particles/
    │   ├─ ParticleEmitterComponent.h
    │   ├─ ParticleSystem.h
    │   └─ ParticleRenderer.h
    │ 
    └─ Debug/
        └─ DebugDraw.h
```

Pas besoin de tout faire maintenant. Mais c’est une bonne direction.

## Priorités concrètes pour `Render/`

### Phase 1 — Nettoyage sans tout casser

| Tâche                                                                                                 | Complexité | Importance | Pourquoi                                                                                                                             |
|:------------------------------------------------------------------------------------------------------|:----------:|:----------:|:-------------------------------------------------------------------------------------------------------------------------------------|
| Déplacer `RenderState`, `BlendFactor`, `CullMode`, `PrimitiveType`, RenderLayer hors de `Drawable.h`  |    S/M     |     4      | Ces concepts ne sont pas propres à `Drawable`. Actuellement ils sont définis dans `Drawable.h`.                                      |
| Renommer `RenderCommande` en `RenderCommand` ou `Renderer`                                            |     S      |     2      | Pas vital, mais `Commande` mélange FR/EN et fait moins API stable.                                                                   |
| Extraire `DrawCommand` dans un header interne                                                         |    S/M     |     4      | Aujourd’hui `DrawCommand` est caché dans `RenderCommande.cpp`.  Pour un pipeline propre, il faut clarifier ce qu’est une commande.   |
| Remplacer le vector global par une RenderQueue                                                        |     M      |     5      | Le `std::vector<DrawCommand>` global est simple mais peu extensible.                                                                 |
| Garder les fonctions `Submit(...)` mais les faire déléguer à une instance de renderer                 |     M      |     4      | Permet de garder l’API actuelle tout en préparant une architecture moins statique.                                                   |
| Corriger le draw instancing                                                                           |    S/M     |     4      | `glDrawArraysInstanced` semble utiliser `nbrInstance` comme vertex count et `indexCount` comme instance count.  À vérifier/corriger. |
| Ajouter asserts/logs sur material/shader/mesh null                                                    |     S      |     3      | Pour éviter des crashes silencieux dans `FlushDrawCommand`.                                                                          |

### Phase 2 — Préparer Vulkan

| Tâche                                                           | Complexité | Importance | Pourquoi                                                                 |
|:----------------------------------------------------------------|:-----------|:-----------|:-------------------------------------------------------------------------|
| Créer GraphicsAPI enum : OpenGL, Vulkan                         | S          | 3          | Permet d’annoncer la direction sans tout implémenter.                    |
| Créer interface GraphicsDevice                                  | L          | 5          | Base obligatoire pour Vulkan.                                            |
| Créer BufferHandle, TextureHandle, ShaderHandle, PipelineHandle | L          | 5          | Les ressources Vulkan ne se manipulent pas comme des IDs OpenGL simples. |
| Séparer Shader runtime et ShaderAsset                           | M/L        | 4          | Vulkan aura probablement SPIR-V, reflection shader, pipelines.           |
| Séparer Material et PipelineState                               | L          | 5          | Vulkan demande une notion beaucoup plus explicite de pipeline.           |
| Créer un système de descriptor/bind groups abstrait             | XL         | 5          | Indispensable pour Vulkan propre, mais à faire plus tard.                |
| Isoler tous les appels gl* dans Backend/OpenGL                  | L/XL       | 5          | Objectif final avant Vulkan.                                             |

### Phase 3 — Préparer les particules

| Tâche                                                 | Complexité | Importance                 | Pourquoi                                                                |
|:------------------------------------------------------|:-----------|:---------------------------|:------------------------------------------------------------------------|
| Ajouter ParticleEmitterComponent                      | M          | 4                          | Côté ECS, définit spawn rate, lifetime, velocity, color, size, texture. |
| Ajouter ParticleSystem                                | M/L        | 4                          | Update CPU des particules au début.                                     |
| Ajouter ParticleRenderer                              | M/L        | 4                          | Render des billboards via instancing.                                   |
| Ajouter RenderLayer::Particles ou render queue dédiée | S/M        | 4                          | Les particules transparentes doivent être triées/rendues séparément.    |
| Ajouter matériau particule                            | M          | 3                          | Shader billboard, texture atlas, blending.                              |
| Ajouter culling/bounds particules                     | M          | 3                          | Important pour perf dès qu’il y a beaucoup de particules.               |
| Plus tard : simulation GPU/compute                    | XL         | 2 maintenant / 5 plus tard | À ne pas faire tout de suite. D’abord CPU + instancing.                 |

## Ordre recommandé sur 4 étapes

### Étape 1 — Stabilisation immédiate

À faire en premier :
 1. corriger les bugs visibles ;
 2. utiliser TransformComponent pour soumettre les meshes ;
 3. aligner CMake ;
 4. sécuriser l’inspector ;
 5. clarifier RenderState.

Objectif : ne pas construire du neuf sur des bases fragiles.

### Étape 2 — Architecture moteur

Ensuite :
 1. séparer `World::Update` en systèmes ;
 2. créer un registre de composants ;
 3. rendre l’inspector data-driven ;
 4. commencer la sérialisation de scènes ;
 5. créer UUID entités/assets.

Objectif : rendre l’engine extensible côté gameplay/editor.

### Étape 3 — Refactor Render propre
Ensuite :

 1. créer RenderQueue;
 2. extraire DrawCommand;
 3. créer Renderer instance-based ;
 4. créer GraphicsDevice minimal ;
 5. déplacer OpenGL vers Backend/OpenGL.

Objectif : ne plus être prisonnier d’OpenGL partout.

### Étape 4 — Nouvelles features

Là seulement :
 1. système de particules ;
 2. viewport editor avancée ;
 3. asset browser ;
 4. dynamic lib/game module ;
 5. Vulkan expérimental.

Objectif : ajouter des features sans rendre le moteur instable.

## Les 10 tâches que je ferais en premier

Si tu veux une liste très concrète, je commencerais par ça :

| Ordre | Tâche                                                                           | Complexité | Importance |
|:-----:|:--------------------------------------------------------------------------------|:----------:|:----------:|
|   1   | Corriger `World::Update` pour utiliser le transform dans RenderCommande::Submit |     S      |     5      |
|   2   | Corriger TransformComponent::SetScale                                           |     S      |     5      |
|   3   | Corriger les casts UInt8 / Int de l’inspector                                   |     S      |     5      |
|   4   | Sécuriser l’édition des strings dans l’inspector                                |     S      |     4      |
|   5   | Sortir RenderState de Drawable.h vers RenderState.h                             |    S/M     |     4      |
|   6   | Créer RenderQueue et déplacer le vector<DrawCommand> dedans                     |     M      |     5      |
|   7   | Créer DrawCommand.h ou RenderCommandData.h interne                              |    S/M     |     4      |
|   8   | Séparer Render.cpp en Window / OpenGLContext / Input callbacks                  |    M/L     |     4      |
|   9   | Créer WorldSystem + déplacer script/transform/render submission progressivement |     L      |     5      |
|  10   | Créer un format de scène minimal JSON                                           |     L      |     5      |

# Roadmap Render/ vers une architecture extensible

Objectif : rendre `Engine/Render/` plus facile à faire évoluer pour ajouter de nouvelles features graphiques sans rester bloqué par l'implémentation OpenGL actuelle, puis préparer une expérimentation Vulkan en dernier.

## Étape 1 — Stabiliser l'existant

- Corriger les chemins de rendu qui ignorent des données ECS importantes, notamment le transform des entités rendues.
- Ajouter des assertions ou logs pour les `Mesh`, `Material`, `Shader` et `Texture` null avant les draw calls.
- Corriger et documenter les chemins instanciés pour éviter de mélanger vertex count, index count et instance count.
- Centraliser les types d'état de rendu (`RenderState`, `BlendFactor`, `CullMode`, `PrimitiveType`, `RenderLayer`) dans un header de rendu commun plutôt que dans une ressource spécifique.
- Ajouter quelques scènes ou commandes de test simples : mesh opaque, mesh transparent, texture, framebuffer, instancing, debug draw.

## Étape 2 — Clarifier les frontières de Render/

- Séparer clairement les responsabilités : core renderer, backend GPU, ressources assets, passes de rendu, debug, particules.
- Extraire `DrawCommand` dans un type explicite et testable.
- Remplacer le stockage global de commandes par une `RenderQueue` possédée par le renderer.
- Garder une façade de compatibilité `RenderCommand::Submit(...)`, mais la faire déléguer à une instance de renderer ou à un contexte courant.
- Déplacer les appels `gl*` restants hors du core renderer vers des classes OpenGL dédiées.

## Étape 3 — Introduire un backend abstrait minimal

- Ajouter un enum `GraphicsAPI` avec au minimum `OpenGL` et `Vulkan` pour rendre le choix de backend explicite.
- Créer une interface `IGraphicsDevice` ou `GraphicsDevice` responsable de l'initialisation, des buffers, textures, shaders, pipelines et commandes de draw.
- Définir des handles opaques (`BufferHandle`, `TextureHandle`, `ShaderHandle`, `PipelineHandle`) au lieu d'exposer des IDs OpenGL.
- Faire d'OpenGL le premier backend concret : `OpenGLGraphicsDevice`, `OpenGLBuffer`, `OpenGLTexture`, `OpenGLShader`, `OpenGLPipeline`.
- Ajouter une factory de backend afin que le reste du moteur ne construise jamais directement des classes OpenGL.

## Étape 4 — Structurer le pipeline de rendu

- Définir une interface `RenderPass` avec une entrée claire : contexte, render queue, caméra, frame data.
- Créer au moins les passes `GeometryPass`, `TransparentPass`, `OverlayPass`, `DebugPass` et `UIPass`.
- Rendre les queues explicites : opaque, transparent, overlay, debug, particles, UI.
- Déplacer le tri transparent, les états de blending/culling/depth et les bindings de matériaux dans les passes concernées.
- Préparer les passes futures : shadow maps, post-process, viewport editor, picking, gizmos.

## Étape 5 — Rendre les ressources plus robustes

- Séparer les assets disque (`TextureAsset`, `ShaderAsset`, `MeshAsset`) des ressources runtime GPU.
- Introduire un `ResourceManager` ou des `AssetHandle` persistants pour éviter de passer partout des `shared_ptr` mutables.
- Rendre `Material` moins couplé à OpenGL en séparant paramètres de matériau, textures, shader asset et pipeline state.
- Préparer un système de shader reflection pour automatiser uniforms, samplers et layouts.
- Ajouter un chemin de hot reload shader/texture plus tard, une fois les handles stables.

## Étape 6 — Ajouter les nouvelles features sur cette base

- Particules : créer `ParticleEmitterComponent`, `ParticleSystem` CPU, puis `ParticleRenderer` via instancing/billboards.
- Viewport editor : rendre dans un framebuffer dédié au lieu de dépendre uniquement de la fenêtre principale.
- Debug draw : isoler lignes, boxes, spheres, bounds et gizmos dans une queue ou une pass séparée.
- Post-process : ajouter une chaîne de passes plein écran après la pass principale.
- Shadows et lighting avancé : ajouter les ressources et passes seulement quand le pipeline est déjà découpé.

## Étape 7 — Préparer Vulkan en dernier

- Vérifier que le core renderer ne contient plus d'appels OpenGL directs.
- Remplacer les chemins implicites par des objets explicites : swapchain, command buffer, render pass, framebuffer, pipeline, descriptor/bind group.
- Compiler les shaders vers SPIR-V ou prévoir une étape de compilation cross-platform.
- Définir les layouts de ressources par pipeline au lieu de pousser des uniforms ad hoc.
- Ajouter un backend `VulkanGraphicsDevice` expérimental derrière la même interface que le backend OpenGL.
- Commencer par une scène minimale Vulkan : clear screen, triangle, mesh simple, texture, puis seulement ensuite le pipeline complet.

## Ordre recommandé

| Ordre | Chantier                                     | Résultat attendu                                                        |
|:-----:|:---------------------------------------------|:------------------------------------------------------------------------|
|   1   | Stabilisation render actuelle                | Moins de bugs et de crashes lors des draw calls                         |
|   2   | `RenderQueue` + `DrawCommand` explicite      | Les soumissions deviennent inspectables et triables                     |
|   3   | `GraphicsDevice` abstrait                    | Le moteur ne dépend plus directement d'OpenGL                           |
|   4   | Ressources GPU par handles                   | Les ressources deviennent compatibles avec plusieurs backends           |
|   5   | Render passes                                | Les nouvelles features s'ajoutent sans modifier un gros renderer global |
|   6   | Particules / viewport / debug / post-process | Les features graphiques arrivent sur une base stable                    |
|   7   | Backend Vulkan expérimental                  | Vulkan est ajouté comme backend, pas comme refactor géant               |

## Définition de terminé avant Vulkan

- Le dossier `Render/Core` ne contient aucun appel direct à OpenGL.
- Les ressources manipulées par le moteur sont des handles ou interfaces, pas des IDs OpenGL publics.
- Les passes de rendu peuvent être ajoutées sans modifier la logique centrale de soumission.
- OpenGL fonctionne encore comme backend de référence.
- Une scène de test couvre opaque, transparent, texture, framebuffer, instancing et UI/debug.