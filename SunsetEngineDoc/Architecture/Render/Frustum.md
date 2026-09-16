(Engine/Render/Resources/Camera.h)
## Concept

[[Frustum]] represente le volume visible d'une [[Camera]].

Il contient six plans:

```cpp
glm::vec4 planes[6]; // left, right, bottom, top, near, far
```

Il peut tester si une [[AABB]] est visible.

```cpp
if (frustum.IsVisible(box))
{
	// submit draw
}
```

Ce test sert au culling avant d'envoyer les objets au [[Renderer]].

