(Editor/Layers/EditorLayer.h)

## Concept

[[EditorLayer]] est le layer principal de l'editeur.

Il possede le [[World]] actif, les panels editor, le viewport et le [[RenderTarget]] utilise pour afficher la scene dans ImGui.

```cpp
class EditorLayer : public Layer
{
	std::shared_ptr<World> m_World;
	std::unique_ptr<WorldHierarchyPanel> m_WorldHierarchy;
	ContentBrowserPanel m_ContentBrowserPanel;
	std::shared_ptr<RenderTarget> m_Framebuffer;
	RenderScene m_RenderScene;
	BuildRenderScene m_BuildRenderScene;
	Camera m_Camera;
};
```

## Viewport

Le viewport rend le [[World]] dans un [[RenderTarget]], puis affiche la color attachment avec `ImGui::Image`.

Le framebuffer OpenGL est affiche avec les UV inverses en Y:

```cpp
ImGui::Image(
	m_Framebuffer->GetColorAttachmentRenderID(),
	viewportSize,
	ImVec2(0.0f, 1.0f),
	ImVec2(1.0f, 0.0f)
);
```

Cela aligne l'orientation ecran avec le world et avec [[ImGuizmo]].

La camera du viewport utilise l'aspect ratio reel du panel, via `Camera::SetAspectRatio`.

## Gizmo

L'editeur utilise [[ImGuizmo]] pour manipuler l'entity selectionnee.

Conditions:

- une entity doit etre selectionnee dans [[WorldHierarchyPanel]] ou via le viewport;
- l'entity doit avoir un [[TransformComponent]];
- le gizmo est dessine en overlay dans la fenetre Viewport.

Raccourcis dans le viewport:

- `T`: translate
- `R`: rotate
- `Y`: scale

Le gizmo manipule une matrice, puis l'editeur la decompose pour mettre a jour:

- `TransformComponent::Position`
- `TransformComponent::Rotation`
- `TransformComponent::Scale`

## Selection viewport

Un clic gauche dans le viewport peut selectionner une entity.

La selection actuelle est une selection approximative pour les sprites:

- elle parcourt les entities avec [[TransformComponent]] et [[SpriteRenderComponent]];
- elle projette le quad local du sprite dans le viewport;
- elle teste si la souris est dans le quad projete;
- elle synchronise la selection avec [[WorldHierarchyPanel]].

Cette selection devra etre remplacee par un framebuffer d'ID pour supporter proprement les meshes 3D, les objets superposes et les cas complexes.

## Open / Save

`File > Open` utilise [[FileDialog]] pour ouvrir une fenetre native du systeme et choisir un fichier `.bin`.

Quand un world est ouvert avec succes:

```cpp
SaveSystem::Load(path, *m_World);
m_ContentBrowserPanel.SetCurrentWorldPath(path);
```

`File > Save` sauvegarde dans le world courant si un chemin existe.

Si aucun chemin n'existe, l'editeur ouvre une popup `Save World As` et cree un fichier dans `CONTENT_PATH`.

Open et Save sont desactives pendant le mode Play.

## Play / Stop

[[EditorLayer]] a deux modes:

```cpp
enum class EditorMode
{
	Edit,
	Play
};
```

En mode Edit:

- le [[World]] est editable;
- les scripts ne sont pas updates;
- `World::Update(dt)` n'est pas appele.

Au Play:

- l'editeur conserve le world d'edition dans `m_EditorWorld`;
- il cree un runtime world avec `World::Clone()`;
- il rend et update ce runtime world;
- les panels sont reconnectes au runtime world.

Au Stop:

- les scripts runtime recoivent `OnEndPlay`;
- le runtime world est detruit;
- l'editeur revient au world d'edition original;
- les modifications faites pendant le Play ne sont pas conservees.

## Runtime scripts

Pendant le mode Play:

```cpp
m_World->BeginInput();
m_World->Update(dt);
```

Les events sont aussi forward au [[World]]:

```cpp
if (m_EditorMode == EditorMode::Play)
	m_World->OnEvent(event);
```

Cela permet aux [[InputComponent]] et aux [[NativeScriptComponent]] de fonctionner pendant le runtime.

