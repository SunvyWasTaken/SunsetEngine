(Engine/Render/Core/BuildRenderScene.h)
## Concept

[[BuildRenderScene]] transforme un [[World]] en [[RenderScene]].

```cpp
BuildRenderScene build;
build(world, scene);
```

Il parcourt les entities et recupere les components necessaires:

- [[TransformComponent]]
- [[SpriteRenderComponent]]
- [[CameraComponent]]

Le but est de separer la logique gameplay du format attendu par le [[Renderer]].

