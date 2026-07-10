# Refactor du [[World]] pour le rendre plus générique

## Objectif

Le but est de transformer [[World]] en conteneur générique de simulation.

Il ne doit plus contenir de logique spécifique à un jeu, au rendu OpenGL, au réseau concret, ni à l’éditeur.

Le [[World]] doit seulement gérer :

* les entités ;
* les composants ;
* les systèmes ;
* les scènes ;
* les événements liés au monde ;
* le cycle de vie runtime.

Le gameplay spécifique doit être injecté via :

* des [[Component]] ;
* des [[System]] ;
* des [[ScriptEntity]] ;
* des [[WorldSubsystem]] ;
* un [[GameModule]] ;
* une [[Scene]].

---

# 1. Ne plus posséder le [[World]] dans un [[Layer]]

## Problème actuel

Actuellement, un [[Layer]] peut créer son propre [[World]].

Exemple problématique :

```cpp
class EditorLayer : public Layer
{
private:
    std::shared_ptr<World> m_World;
};
```

Cela crée un problème pour l’éditeur, car le [[World]] affiché dans la hierarchy peut être différent du [[World]] réellement utilisé par le jeu.

## Nouvelle règle

Un [[Layer]] ne possède pas le [[World]].

Un [[Layer]] utilise le [[World]].

Le [[World]] doit être possédé par un objet plus haut niveau :

```cpp
[[Application]]
 └── [[GameInstance]] / [[EditorContext]]
      └── [[WorldManager]]
           └── ActiveWorld
```

## Changement à faire

Créer un [[WorldManager]].

```cpp
class WorldManager
{
public:
    std::shared_ptr<World> CreateWorld()
    {
        m_ActiveWorld = std::make_shared<World>();
        return m_ActiveWorld;
    }

    void SetActiveWorld(std::shared_ptr<World> world)
    {
        m_ActiveWorld = std::move(world);
    }

    std::shared_ptr<World> GetActiveWorld()
    {
        return m_ActiveWorld;
    }

    World* GetActiveWorldRaw()
    {
        return m_ActiveWorld.get();
    }

private:
    std::shared_ptr<World> m_ActiveWorld;
};
```

Puis dans [[GameInstance]] :

```cpp
class GameInstance
{
public:
    WorldManager& GetWorldManager()
    {
        return m_WorldManager;
    }

private:
    WorldManager m_WorldManager;
};
```

---

# 2. Faire update le [[World]] dans [[Application]]

## Décision

Oui, il est cohérent de faire update le [[World]] directement dans [[Application]], avant les layers.

Cela donne une boucle plus propre :

```cpp
Application::Run()
{
    while (running)
    {
        PollEvents();

        UpdateNetwork(dt);

        UpdateWorld(dt);

        UpdateLayers(dt);

        RenderWorld();

        DrawLayers();
    }
}
```

Mais il faut être prudent : [[Application]] ne doit pas connaître le gameplay.

Elle doit seulement demander au [[GameInstance]] quel est le [[World]] actif.

## Exemple

```cpp
void Application::UpdateWorld(float dt)
{
    if (!m_GameInstance)
        return;

    World* world = m_GameInstance->GetWorldManager().GetActiveWorldRaw();

    if (!world)
        return;

    world->Update(dt);
}
```

Puis dans la boucle :

```cpp
void Application::Run()
{
    while (IsRunning())
    {
        float dt = ComputeDeltaTime();

        if (NetworkService::IsInitialized())
            NetworkService::Get().Update(dt);

        UpdateWorld(dt);

        for (auto& layer : m_LayerStack)
            layer->OnUpdate(dt);

        if (!IsHeadless())
        {
            RenderCommande::BeginFrame();

            RenderWorld();

            for (auto layer = m_LayerStack.end(); layer != m_LayerStack.begin();)
                (*--layer)->OnDraw();

            RenderCommande::EndFrame();
        }
    }
}
```

## Remarque importante

Si les layers servent encore à faire du gameplay, alors `World::Update()` avant `Layer::OnUpdate()` peut poser problème.

La règle recommandée est :

```cpp
Gameplay = dans le World
Editor / UI / Debug / Tools = dans les Layers
```

Donc les layers ne devraient plus contenir la logique principale du jeu.

---

# 3. Séparer update du monde et rendu du monde

## Problème actuel

Le [[World]] ne doit pas appeler directement le renderer.

À éviter :

```cpp
RenderCommande::Submit(mesh.m_mesh);
```

dans :

```cpp
World::Update(float dt)
```

Cela couple [[World]] au rendu.

## Nouvelle règle

[[World::Update]] met à jour la simulation.

[[Renderer]] ou [[WorldRenderer]] s’occupe du rendu.

## Avant

```cpp
void World::Update(float dt)
{
    UpdateScripts(dt);
    UpdateTransforms(dt);

    for (auto entity : renderableEntities)
    {
        RenderCommande::Submit(mesh);
    }
}
```

## Après

```cpp
void World::Update(float dt)
{
    UpdateSystems(dt);
    UpdateScripts(dt);
    UpdateTransforms(dt);
}
```

Puis :

```cpp
class WorldRenderer
{
public:
    void Render(World& world)
    {
        world.EachEntityWith<TransformComponent, MeshComponent>(
            [](Entity entity, TransformComponent& transform, MeshComponent& mesh)
            {
                RenderCommande::Submit(mesh.m_mesh);
            }
        );
    }
};
```

Dans [[Application]] :

```cpp
if (!Application::IsHeadless())
{
    RenderCommande::BeginFrame();

    if (World* world = GetActiveWorld())
        m_WorldRenderer.Render(*world);

    DrawLayers();

    RenderCommande::EndFrame();
}
```

---

# 4. Ajouter une API propre pour itérer les entités

## Problème

L’éditeur accède directement à `m_Registry`.

Cela marche, mais cela couple fortement l’éditeur à EnTT.

## Changement à faire

Ajouter des méthodes publiques dans [[World]].

```cpp
class World
{
public:
    template <typename Func>
    void EachEntity(Func&& func)
    {
        m_Registry.each([&](entt::entity handle)
        {
            func(Entity{this, handle});
        });
    }

    template <typename... Components, typename Func>
    void Each(Func&& func)
    {
        auto view = m_Registry.view<Components...>();

        for (auto entity : view)
        {
            func(Entity{this, entity}, view.template get<Components>(entity)...);
        }
    }

private:
    entt::registry m_Registry;
};
```

Utilisation côté éditeur :

```cpp
void WorldHierarchyPanel::OnImGuiRender()
{
    ImGui::Begin("Hierarchy");

    if (m_Context)
    {
        m_Context->EachEntity([&](Entity entity)
        {
            DrawNodeEntity(entity);
        });
    }

    ImGui::End();
}
```

---

# 5. Supprimer le `friend class WorldHierarchyPanel`

## Problème

Si [[WorldHierarchyPanel]] est `friend` de [[World]], l’éditeur peut accéder aux détails internes du monde.

Cela casse l’encapsulation.

## Avant

```cpp
class World
{
    friend class WorldHierarchyPanel;

private:
    entt::registry m_Registry;
};
```

## Après

```cpp
class World
{
public:
    template <typename Func>
    void EachEntity(Func&& func);

    template <typename... Components, typename Func>
    void Each(Func&& func);

private:
    entt::registry m_Registry;
};
```

L’éditeur utilise l’API publique du [[World]].

---

# 6. Ajouter un vrai système de hiérarchie

## Problème actuel

La hierarchy affiche une liste plate d’entités.

Pour avoir une vraie hierarchy type Unity/Unreal, il faut représenter les relations parent/enfant.

## Nouveau composant

```cpp
struct RelationshipComponent : public Component
{
    entt::entity Parent = entt::null;
    std::vector<entt::entity> Children;
};
```

## API dans [[World]]

```cpp
void World::SetParent(Entity child, Entity parent)
{
    auto& childRel = child.GetOrAddComponent<RelationshipComponent>();

    if (childRel.Parent != entt::null)
    {
        Entity oldParent{this, childRel.Parent};

        if (auto* oldParentRel = oldParent.GetComponent<RelationshipComponent>())
        {
            std::erase(oldParentRel->Children, static_cast<entt::entity>(child));
        }
    }

    childRel.Parent = static_cast<entt::entity>(parent);

    auto& parentRel = parent.GetOrAddComponent<RelationshipComponent>();
    parentRel.Children.push_back(static_cast<entt::entity>(child));
}
```

## Affichage récursif dans [[WorldHierarchyPanel]]

```cpp
void WorldHierarchyPanel::DrawEntityRecursive(Entity entity)
{
    auto* tag = entity.GetComponent<TagComponent>();
    const char* name = tag ? tag->Tag.c_str() : "Unnamed Entity";

    bool opened = ImGui::TreeNodeEx(
        reinterpret_cast<void*>(static_cast<std::uint64_t>(static_cast<std::uint32_t>(entity))),
        ImGuiTreeNodeFlags_OpenOnArrow,
        "%s",
        name
    );

    if (ImGui::IsItemClicked())
        m_SelectedEntity = entity;

    if (opened)
    {
        if (auto* rel = entity.GetComponent<RelationshipComponent>())
        {
            for (entt::entity child : rel->Children)
            {
                DrawEntityRecursive(Entity{m_Context.get(), child});
            }
        }

        ImGui::TreePop();
    }
}
```

---

# 7. Retirer le réseau concret du [[World]]

## Problème

Le [[World]] ne devrait pas directement initialiser ou shutdown le réseau.

À éviter dans [[World]] :

```cpp
NetworkService::Get().RegisterHandler(...);
NetworkService::Shutdown();
```

Le réseau est un service externe ou un subsystem.

## Nouvelle solution

Créer un [[NetworkWorldSubsystem]].

```cpp
class IWorldSubsystem
{
public:
    virtual ~IWorldSubsystem() = default;

    virtual void OnAttach(World& world) {}
    virtual void OnDetach(World& world) {}
    virtual void OnUpdate(World& world, float dt) {}
};
```

Puis :

```cpp
class NetworkWorldSubsystem : public IWorldSubsystem
{
public:
    void OnAttach(World& world) override
    {
        if (!NetworkService::IsInitialized())
            return;

        NetworkService::Get().RegisterHandler<NetworkPlayerSessionMessage>(
            [&world](PeerId peerId, const NetworkPlayerSessionMessage& msg)
            {
                // traiter message
            }
        );
    }

    void OnUpdate(World& world, float dt) override
    {
        // sync réseau si besoin
    }
};
```

Dans [[World]] :

```cpp
class World
{
public:
    template <typename T, typename... Args>
    T& AddSubsystem(Args&&... args)
    {
        auto subsystem = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *subsystem;

        subsystem->OnAttach(*this);
        m_Subsystems.emplace_back(std::move(subsystem));

        return ref;
    }

private:
    std::vector<std::unique_ptr<IWorldSubsystem>> m_Subsystems;
};
```

---

# 8. Remplacer la logique hardcodée par des [[System]]

## Problème

[[World::Update]] risque de devenir une grosse fonction qui connaît tous les composants.

À éviter :

```cpp
void World::Update(float dt)
{
    UpdateInput(dt);
    UpdateScripts(dt);
    UpdateTransforms(dt);
    UpdatePhysics(dt);
    UpdateChunks(dt);
    UpdateInventory(dt);
}
```

## Nouvelle solution

Créer une interface de système.

```cpp
class IWorldSystem
{
public:
    virtual ~IWorldSystem() = default;
    virtual void Update(World& world, float dt) = 0;
};
```

Puis :

```cpp
class ScriptSystem : public IWorldSystem
{
public:
    void Update(World& world, float dt) override
    {
        world.Each<NativeScriptComponent>(
            [dt](Entity entity, NativeScriptComponent& script)
            {
                if (!script.m_ScriptEntity)
                {
                    script.m_ScriptEntity = script.InstantiateScriptEntity();
                    script.m_ScriptEntity->m_Entity = entity;
                }

                script.m_ScriptEntity->OnUpdate(dt);
            }
        );
    }
};
```

```cpp
class TransformSystem : public IWorldSystem
{
public:
    void Update(World& world, float dt) override
    {
        world.Each<TransformComponent>(
            [dt](Entity entity, TransformComponent& transform)
            {
                transform.Update(dt);
            }
        );
    }
};
```

Dans [[World]] :

```cpp
class World
{
public:
    template <typename T, typename... Args>
    T& AddSystem(Args&&... args)
    {
        auto system = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *system;
        m_Systems.emplace_back(std::move(system));
        return ref;
    }

    void Update(float dt)
    {
        for (auto& subsystem : m_Subsystems)
            subsystem->OnUpdate(*this, dt);

        for (auto& system : m_Systems)
            system->Update(*this, dt);
    }

private:
    std::vector<std::unique_ptr<IWorldSystem>> m_Systems;
    std::vector<std::unique_ptr<IWorldSubsystem>> m_Subsystems;
};
```

---

# 9. Créer un [[GameModule]] pour remplir le [[World]]

## Objectif

Le [[World]] reste générique.

Le jeu ajoute ses systèmes et composants spécifiques.

## Interface

```cpp
class GameModule
{
public:
    virtual ~GameModule() = default;

    virtual void RegisterComponents(World& world) {}
    virtual void RegisterSystems(World& world) {}
    virtual void CreateDefaultScene(World& world) {}
};
```

## Exemple côté jeu

```cpp
class SunsetCraftModule : public GameModule
{
public:
    void RegisterSystems(World& world) override
    {
        world.AddSystem<ScriptSystem>();
        world.AddSystem<TransformSystem>();
        world.AddSystem<ChunkStreamingSystem>();
        world.AddSystem<PlayerControllerSystem>();
        world.AddSystem<InventorySystem>();
    }

    void CreateDefaultScene(World& world) override
    {
        Entity player = world.CreateEntity("Player");

        player.AddComponent<TransformComponent>();
        player.AddComponent<InputComponent>();
        player.AddComponent<PlayerComponent>();
    }
};
```

## Utilisation

```cpp
auto world = gameInstance.GetWorldManager().CreateWorld();

gameModule.RegisterComponents(*world);
gameModule.RegisterSystems(*world);
gameModule.CreateDefaultScene(*world);
```

---

# 10. Ajouter un état de simulation

## Objectif

L’éditeur doit pouvoir afficher un [[World]] sans forcément le simuler.

Il faut distinguer :

* édition ;
* play ;
* pause ;
* step frame.

## Enum

```cpp
enum class WorldSimulationState
{
    Edit,
    Play,
    Pause
};
```

Dans [[World]] :

```cpp
class World
{
public:
    void SetSimulationState(WorldSimulationState state)
    {
        m_State = state;
    }

    WorldSimulationState GetSimulationState() const
    {
        return m_State;
    }

    void Update(float dt)
    {
        if (m_State == WorldSimulationState::Pause)
            return;

        if (m_State == WorldSimulationState::Edit)
        {
            UpdateEditorSystems(dt);
            return;
        }

        UpdateRuntimeSystems(dt);
    }

private:
    WorldSimulationState m_State = WorldSimulationState::Edit;
};
```

## Variante plus propre

Séparer les systèmes editor et runtime :

```cpp
enum class SystemPhase
{
    Editor,
    Runtime,
    Always
};
```

```cpp
struct WorldSystemEntry
{
    SystemPhase Phase;
    std::unique_ptr<IWorldSystem> System;
};
```

---

# 11. Séparer [[Scene]] et [[World]]

## Rôle de [[World]]

Le [[World]] est une instance runtime.

Il contient un `entt::registry`.

## Rôle de [[Scene]]

La [[Scene]] est une ressource sauvegardable.

Elle représente les entités et composants à charger.

```cpp
class Scene
{
public:
    std::string Name;
    std::filesystem::path Path;
};
```

Le [[World]] peut charger une [[Scene]] :

```cpp
class World
{
public:
    void LoadScene(const Scene& scene);
    Scene SaveToScene() const;
};
```

## Pourquoi faire ça

Cela permet d’avoir :

```cpp
Editor
 └── ouvre une Scene

Runtime
 └── instancie un World depuis une Scene
```

Et plus tard :

```cpp
Play In Editor
 ├── EditorWorld
 └── RuntimeWorld = copie de EditorWorld
```

---

# 12. Préparer le Play In Editor

## Objectif

Quand tu appuies sur Play dans l’éditeur, il ne faut pas forcément modifier directement le monde édité.

Il vaut mieux copier le monde.

```cpp
EditorContext
{
    std::shared_ptr<World> EditorWorld;
    std::shared_ptr<World> RuntimeWorld;
    std::shared_ptr<World> ActiveWorld;
};
```

## Mode édition

```cpp
ActiveWorld = EditorWorld;
```

## Mode play

```cpp
RuntimeWorld = EditorWorld->Clone();
ActiveWorld = RuntimeWorld;
ActiveWorld->SetSimulationState(WorldSimulationState::Play);
```

## Stop play

```cpp
RuntimeWorld.reset();
ActiveWorld = EditorWorld;
```

Cela évite que le mode play casse la scène éditée.

---

# 13. Ajouter `Clone()` au [[World]]

## Objectif

Pouvoir dupliquer un monde pour le mode play.

```cpp
std::shared_ptr<World> World::Clone() const
{
    auto clone = std::make_shared<World>();

    // TODO:
    // - copier les entités
    // - copier les composants
    // - reconstruire les relations parent/enfant
    // - ne pas copier les systèmes runtime dangereux
    // - réinitialiser les scripts

    return clone;
}
```

Au début, tu peux faire simple avec une sauvegarde temporaire :

```cpp
Scene scene = world.SaveToScene();
auto runtimeWorld = std::make_shared<World>();
runtimeWorld->LoadScene(scene);
```

---

# 14. Ajouter une API `CreateEntity` plus complète

## Actuel

```cpp
Entity CreateEntity(const std::string& name);
```

## Version recommandée

```cpp
Entity World::CreateEntity(const std::string& name = "Entity")
{
    Entity entity{this, m_Registry.create()};

    entity.AddComponent<TagComponent>(name);
    entity.AddComponent<TransformComponent>();
    entity.AddComponent<RelationshipComponent>();

    return entity;
}
```

Pour les entités internes :

```cpp
Entity World::CreateRawEntity()
{
    return Entity{this, m_Registry.create()};
}
```

---

# 15. Ajouter `DestroyEntity`

## Objectif

Un [[World]] générique doit gérer la destruction propre des entités.

```cpp
void World::DestroyEntity(Entity entity)
{
    if (!entity)
        return;

    if (auto* rel = entity.GetComponent<RelationshipComponent>())
    {
        for (entt::entity child : rel->Children)
        {
            DestroyEntity(Entity{this, child});
        }

        if (rel->Parent != entt::null)
        {
            Entity parent{this, rel->Parent};

            if (auto* parentRel = parent.GetComponent<RelationshipComponent>())
            {
                std::erase(parentRel->Children, static_cast<entt::entity>(entity));
            }
        }
    }

    m_Registry.destroy(static_cast<entt::entity>(entity));
}
```

---

# 16. Ajouter `GetOrAddComponent` dans [[Entity]]

## Objectif

Simplifier les appels génériques.

```cpp
template <typename T, typename... Args>
T& GetOrAddComponent(Args&&... args)
{
    if (auto* component = GetComponent<T>())
        return *component;

    return AddComponent<T>(std::forward<Args>(args)...);
}
```

---

# 17. Ne pas faire de classe enfant de [[World]] pour chaque jeu

## Règle

Éviter :

```cpp
class SunsetCraftWorld : public World
{
};
```

Préférer :

```cpp
world.AddSystem<ChunkStreamingSystem>();
world.AddSystem<PlayerControllerSystem>();
world.AddSubsystem<NetworkWorldSubsystem>();
```

## Pourquoi

Si chaque jeu a son propre `World` enfant, l’éditeur devra connaître le type réel du monde.

Cela pousse vers :

```cpp
dynamic_cast<SunsetCraftWorld*>(world)
```

Ce qui rend l’éditeur moins générique.

Le bon modèle est :

```cpp
World générique
+ Components spécifiques
+ Systems spécifiques
+ Subsystems spécifiques
+ GameModule spécifique
```

---

# 18. Nouvelle responsabilité de chaque classe

## [[Application]]

Responsabilités :

* boucle principale ;
* timing ;
* update réseau global ;
* update du monde actif ;
* update des layers ;
* rendu ;
* command buffer ;
* fermeture application.

Ne contient pas de gameplay.

## [[GameInstance]]

Responsabilités :

* possède le [[WorldManager]] ;
* possède le [[GameModule]] ;
* donne accès au monde actif ;
* configure le monde au démarrage.

## [[WorldManager]]

Responsabilités :

* créer un monde ;
* charger un monde ;
* changer le monde actif ;
* donner accès au monde actif.

## [[World]]

Responsabilités :

* entités ;
* registry ;
* composants ;
* systèmes ;
* subsystems ;
* état de simulation ;
* création/destruction d’entités ;
* relations parent/enfant.

## [[WorldRenderer]]

Responsabilités :

* lire les composants renderables ;
* soumettre les draw commands ;
* ne pas modifier la simulation.

## [[Layer]]

Responsabilités :

* UI ;
* debug ;
* editor tools ;
* viewport ;
* overlays ;
* transitions ;
* menus.

Un [[Layer]] ne doit plus être le propriétaire principal du monde.

---

# 19. Ordre recommandé de la frame

## Runtime simple

```cpp
while (running)
{
    PollEvents();

    NetworkUpdate(dt);

    ExecuteCommandBuffer();

    WorldUpdate(dt);

    LayerUpdate(dt);

    BeginFrame();

    RenderWorld();

    DrawLayers();

    EndFrame();
}
```

## Éditeur

```cpp
while (running)
{
    PollEvents();

    NetworkUpdate(dt);

    ExecuteCommandBuffer();

    EditorPreUpdate(dt);

    if (editorContext.ShouldSimulateWorld())
        ActiveWorld->Update(dt);

    EditorUpdate(dt);

    BeginFrame();

    ViewportRenderWorld();

    EditorDrawPanels();

    EndFrame();
}
```

## Variante avec hooks de layer

Si certains layers doivent agir avant le monde :

```cpp
class Layer
{
public:
    virtual void OnPreWorldUpdate(float dt) {}
    virtual void OnUpdate(float dt) {}
    virtual void OnDraw() {}
};
```

Puis :

```cpp
for (auto& layer : m_LayerStack)
    layer->OnPreWorldUpdate(dt);

UpdateWorld(dt);

for (auto& layer : m_LayerStack)
    layer->OnUpdate(dt);
```

C’est plus flexible pour l’éditeur.

---

# 20. Plan de migration conseillé

## Étape 1

Créer [[WorldManager]].

```cpp
class WorldManager;
```

Ajouter dans [[GameInstance]] :

```cpp
WorldManager m_WorldManager;
```

---

## Étape 2

Retirer `std::shared_ptr<World> m_World` de [[EditorLayer]].

Remplacer par :

```cpp
EditorContext& m_Context;
```

ou récupérer le monde via [[GameInstance]].

---

## Étape 3

Déplacer `World::Update()` dans [[Application]].

Ajouter :

```cpp
void Application::UpdateWorld(float dt);
```

---

## Étape 4

Sortir le rendu de [[World::Update]].

Créer :

```cpp
class WorldRenderer;
```

Puis appeler :

```cpp
m_WorldRenderer.Render(*world);
```

depuis [[Application]] ou [[ViewportLayer]].

---

## Étape 5

Créer [[IWorldSystem]].

Déplacer progressivement :

* script update ;
* transform update ;
* input update ;
* physics update ;
* gameplay update.

---

## Étape 6

Créer [[IWorldSubsystem]].

Déplacer progressivement :

* réseau ;
* chunk streaming ;
* save/load runtime ;
* replication ;
* physics world.

---

## Étape 7

Ajouter [[RelationshipComponent]].

Modifier [[WorldHierarchyPanel]] pour afficher une vraie hierarchy récursive.

---

## Étape 8

Ajouter [[Scene]].

Préparer :

```cpp
World::LoadScene(scene);
World::SaveToScene();
```

---

## Étape 9

Préparer le Play In Editor.

Créer :

```cpp
EditorWorld
RuntimeWorld
ActiveWorld
```

---

# 21. Résultat final attendu

L’architecture cible devient :

```cpp
Application
 ├── GameInstance
 │    ├── WorldManager
 │    │    └── ActiveWorld
 │    │         ├── entt::registry
 │    │         ├── Systems
 │    │         └── Subsystems
 │    └── GameModule
 │
 ├── Renderer
 │    └── WorldRenderer
 │
 └── LayerStack
      ├── EditorLayer
      ├── ViewportLayer
      ├── DebugLayer
      └── UILayer
```

Le monde devient générique.

Le jeu devient un ensemble de composants, systèmes et modules.

L’éditeur peut inspecter n’importe quel monde actif.

Les layers restent utiles, mais ils ne sont plus responsables de posséder la simulation principale.

---

# 22. Résumé des règles finales

```cpp
World = état de la simulation
Layer = outil ou mode d'exécution autour du monde
Application = boucle principale
GameInstance = contexte global du jeu
WorldManager = possession du monde actif
WorldRenderer = rendu du monde
GameModule = injection du gameplay
System = logique runtime
Subsystem = service attaché au monde
Scene = donnée sauvegardable
```

Règle principale :

```cpp
Le World doit être générique.
Le gameplay doit être injecté.
Le rendu doit être externe.
Les layers ne doivent pas posséder le vrai état du jeu.
```
