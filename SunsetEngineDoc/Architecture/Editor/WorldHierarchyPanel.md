(Editor/Panels/WorldHierarchyPanel.h)

## Concept

[[WorldHierarchyPanel]] affiche les [[Entity]] d'un [[World]] et edite leurs components.

```cpp
WorldHierarchyPanel panel(world);
panel.OnImGuiRender();
```

Le panel garde:

- un contexte de [[World]];
- une entity selectionnee.

```cpp
std::shared_ptr<World> m_Context;
Entity m_SelectedEntity;
```

## Selection

Le panel expose la selection courante:

```cpp
Entity GetSelectedEntity() const;
void SetSelectedEntity(const Entity& entity);
```

Cela permet a [[EditorLayer]]:

- d'afficher un gizmo sur l'entity selectionnee;
- de selectionner une entity depuis le viewport;
- de synchroniser la selection entre viewport, hierarchy et inspector.

## Hierarchy

La fenetre `Hierarchy` liste les entities du [[World]].

```cpp
m_Context->ForEach([&](const Entity& entity)
{
	DrawNodeEntity(entity);
});
```

Un clic sur une entity la selectionne.

Un clic dans le vide de la fenetre deselectionne l'entity courante.

## Properties

La fenetre `Properties` affiche les components de l'entity selectionnee.

```cpp
if (m_SelectedEntity)
	DrawComponents(m_SelectedEntity);
```

## Reflection

Pour les components simples, le panel utilise [[ReflectionType]] pour dessiner automatiquement les champs editables.

Types actuellement geres:

- float
- bool
- int
- string
- vec2
- vec3
- vec4
- enum

## Components specialises

Certains components ont un affichage specialise:

- [[TagComponent]]
- [[TransformComponent]]
- [[NativeScriptComponent]]
- [[InputComponent]]

[[NativeScriptComponent]] affiche les scripts instancies et leurs proprietes exposees.

[[InputComponent]] affiche ses bindings et permet de modifier les champs clavier/souris/gamepad.

