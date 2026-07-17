# SunsetEngine

SunsetEngine est un prototype de moteur de jeu C++23 conçu comme une bibliothèque statique modulaire. Il fournit un socle applicatif, un rendu OpenGL, un ECS basé sur EnTT, une pile de layers, un système d'entrées configurable, une couche réseau ENet, une interface retained-mode via SRmGUI, ainsi que des outils de sauvegarde, de profiling et de debug.

Le dépôt contient également un exécutable d'éditeur en cours de construction, destiné à valider les systèmes du moteur dans une application réelle.

---

## Fonctionnalités

- **Cœur applicatif** : boucle principale, fenêtre GLFW, mode headless, pile de layers/overlays, chargement différé de layers, dispatch d'événements et arrêt contrôlé de l'application.
- **Game framework** : `GameInstance`, `World`, entités EnTT, systèmes de monde, scripts natifs et composants réutilisables (`Transform`, `Camera`, `Input`, etc.).
- **Rendu OpenGL** : commandes de rendu, framebuffers, caméras, shaders, textures, matériaux, meshes, drawables, vertex/index/uniform buffers et états de rendu configurables.
- **Pipeline de rendu évolutif** : passes de rendu, couches `World`/`Overlay`, support des overlays 3D après le monde et premières briques pour le rendu transparent/particules.
- **Entrées** : événements clavier/souris, actions nommées chargées depuis JSON, sources d'entrées locales et sources d'entrées réseau.
- **Réseau** : transport ENet, host/join, messages typés, canaux configurables, callbacks de connexion/déconnexion et diffusion fiable ou non fiable.
- **UI** : widgets retained-mode SRmGUI (`Panel`, `Button`, `Text`, `Image`, `Overlay`, `GridPanel`, `VerticalBox`, `HorizontalBox`, `WidgetSwitch`) avec backend OpenGL.
- **Sauvegarde** : archives binaires génériques, sérialisation de types arithmétiques, enums, chaînes, tableaux, vecteurs et types GLM courants.
- **Réflexion légère** : description runtime de champs simples pour construire des outils d'édition et d'inspection.
- **Outils** : logs spdlog, texte de debug à l'écran, scopes de profiling, helpers mathématiques et primitives de debug.

---

## Organisation du dépôt

```text
.
├── CMakeLists.txt                  # Point d'entrée CMake du workspace
├── vcpkg.json                      # Manifeste des dépendances externes
├── Engine/
│   ├── CMakeLists.txt              # Cible statique SunsetEngine
│   ├── SunsetEngine.h              # Include d'entrée pour les APIs principales
│   ├── Backend/OpenGL/             # Implémentation OpenGL bas niveau
│   ├── Core/                       # Application, layers, événements, entrées, GameInstance
│   ├── GameFramework/              # World, Entity, composants, systèmes et scripts natifs
│   ├── Log/                        # Logs et affichage debug à l'écran
│   ├── Math/                       # Primitives mathématiques
│   ├── Network/                    # Service réseau, transport abstrait et backend ENet
│   ├── Platform/                   # Fenêtres et intégration GLFW
│   ├── Reflection/                 # Métadonnées runtime simples
│   ├── Render/                     # Renderer, ressources, buffers, pipeline, particules, debug draw
│   ├── SaveSystem/                 # Archives binaires et helpers de sauvegarde
│   └── Utility/                    # Profiling et fonctions utilitaires
├── Editor/                         # Application d'éditeur/prototype basée sur le moteur
├── Shaders/                        # Shaders GLSL fournis avec le moteur
├── Thirdparty/FastNoiseSIMD/       # Dépendance vendoriée
└── Thirdparty/SRmGUI/              # Bibliothèque UI retained-mode vendoriée
```

---

## Prérequis

- Compilateur compatible C++23.
- CMake 3.28 ou plus récent.
- vcpkg recommandé pour installer les dépendances via le manifeste.
- Pilotes OpenGL pour les applications non headless.

Dépendances déclarées dans `vcpkg.json` :

- assimp
- enet
- entt
- glad
- glfw3
- glm
- imgui avec bindings GLFW, OpenGL 3 et docking
- nlohmann-json
- spdlog

---

## Compilation

Configuration avec vcpkg en mode manifeste :

```bash
git clone <repository-url>
cd SunsetEngine

cmake -S . -B build \
  -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake

cmake --build build
```

Si l'environnement expose déjà les dépendances CMake nécessaires :

```bash
cmake -S . -B build
cmake --build build
```

Pour des chemins d'exécution proches d'un build distribué, configurez avec `-DPRODUCTION_BUILD=ON`. Dans ce mode, `SAVE_PATH` vaut `./Save/` et `ENGINE_SHADERS_PATH` vaut `./Shaders/`. En développement, ces chemins pointent vers l'arborescence source.

> Note : l'éditeur est une application de validation du moteur. Selon l'intégration locale du module de jeu, il peut nécessiter des cibles supplémentaires au moment du link.

---

## Intégration dans un projet CMake

Ajoutez le moteur en sous-répertoire puis liez la cible `SunsetEngine` :

```cmake
add_subdirectory(path/to/SunsetEngine)

target_link_libraries(MyGame PRIVATE SunsetEngine)
```

Exemple minimal :

```cpp
#include <SunsetEngine.h>
#include <Core/Layer.h>

class GameLayer final : public Sunset::Layer
{
public:
    void OnUpdate(float dt) override
    {
        // Update gameplay here.
    }

    void OnDraw() override
    {
        // Submit render commands here.
    }
};

int main()
{
    Sunset::WindowSetting settings;
    settings.WindowSize = {1280, 720};
    settings.WindowTitle = "Sunset Game";

    Sunset::Application app(settings);
    app.PushLayer<GameLayer>();
    app.Run();
}
```

---

## Mode headless

Le mode headless permet d'exécuter la boucle applicative sans créer de fenêtre, de contexte OpenGL, de contexte ImGui ou de frame de rendu. Les layers et le `GameInstance` continuent d'être mis à jour, et `NetworkService` est tické automatiquement lorsqu'il est initialisé.

```cpp
Sunset::WindowSetting settings;
settings.Headless = true;
settings.HeadlessTickRate = 60.0f;

Sunset::Application app(settings);
app.PushLayer<ServerLayer>();
app.Run();
```

---

## Rendu d'éléments après le monde

Pour des mains ou objets de vue première personne, soumettez les drawables du monde normalement, puis appliquez `RenderState::AfterWorldOverlay()` au drawable à afficher après la scène.

```cpp
Sunset::Drawable hand = CreateHandDrawable();
hand.m_RenderState = Sunset::RenderState::AfterWorldOverlay();

Sunset::RenderCommand::Submit(hand, handTransform);
```

Cet état place le drawable dans la couche `RenderLayer::Overlay` et désactive le test/l'écriture de profondeur pour éviter qu'il soit masqué par la géométrie du monde.

---

## Sauvegarde et données runtime

SunsetEngine utilise des définitions de compilation pour localiser les données runtime :

- `SAVE_PATH` : `./Save/` avec `PRODUCTION_BUILD=ON`, sinon `${CMAKE_SOURCE_DIR}/Save/`.
- `ENGINE_SHADERS_PATH` : `./Shaders/` avec `PRODUCTION_BUILD=ON`, sinon le dossier `Shaders/` du moteur.

Le système d'entrées charge `Input.json` depuis `SAVE_PATH` pour les applications non headless. Exemple attendu :

```text
Save/Input.json
```

Le `SaveSystem` fournit également des helpers de sauvegarde/chargement binaire :

```cpp
PlayerData data;
Sunset::SaveSystem::Save("Save/player.bin", data);
Sunset::SaveSystem::Load("Save/player.bin", data);
```

Les types complexes doivent exposer une fonction `Serialize` compatible avec `BinaryInputArchive` et `BinaryOutputArchive`.

---

## Aperçu réseau

La couche réseau est exposée via `Sunset::NetworkService`. Elle prend en charge l'hébergement, la connexion à un hôte, les handlers typés, les canaux de messages et l'envoi fiable ou non fiable.

```cpp
struct PlayerMove
{
    static constexpr Sunset::ChannelId ChannelId = 1;
    float X;
    float Y;
    float Z;
};

Sunset::NetworkService::Init();
auto& network = Sunset::NetworkService::Get();

network.Host(7777, 8);
network.RegisterHandler<PlayerMove>([](Sunset::PeerId peer, const PlayerMove& move) {
    // Handle movement from peer.
});

PlayerMove move{.X = 1.0f, .Y = 0.0f, .Z = 0.0f};
network.Broadcast(move);

Sunset::NetworkService::Shutdown();
```

---

## Notes de développement

- La cible `SunsetEngine` est une bibliothèque statique définie dans `Engine/CMakeLists.txt`.
- Les sources sont collectées avec `file(GLOB_RECURSE ... CONFIGURE_DEPENDS)`.
- `Engine/SunsetPCH.h` est configuré comme precompiled header public.
- `Thirdparty/FastNoiseSIMD` et `Thirdparty/SRmGUI` sont construits comme sous-projets et liés au moteur.
- Le dossier `Engine/` est exposé comme include directory public.

---

## Licence

Ce dépôt inclut un fichier `LICENSE`. Consultez-le avant toute distribution ou réutilisation du moteur.
