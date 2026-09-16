(Editor/Panels/WorldHierarchyPanel.h)
## Concept

[[WorldHierarchyPanel]] affiche le contenu d'un [[World]] dans l'editor.

Il garde un contexte de world et une entity selectionnee.

```cpp
WorldHierarchyPanel panel(world);
panel.OnImGuiRender();
```

## Entity

Le panel liste les [[Entity]] du [[World]].

Quand une entity est selectionnee, il affiche ses components.

```cpp
DrawNodeEntity(entity);
DrawComponents(entity);
```

## Reflection

Pour les components generiques, le panel utilise [[ReflectionType]]
pour dessiner les champs editables.

Les components plus complexes, comme [[InputComponent]], peuvent avoir un dessin ImGui specialise.

