(Engine/Render/Resources/Camera.h)
## Concept

[[Camera]] contient les donnees necessaires pour produire une view matrix et une projection.

```cpp
glm::mat4 projection = camera.GetProjection();
glm::mat4 view = camera.GetViewMatrix();
```

Elle peut etre en perspective ou orthographique.

```cpp
enum class ProjectionType
{
	Perspective,
	Orthographic
};
```

## Frustum

[[Camera]] peut construire un [[Frustum]].

```cpp
Frustum frustum = camera.GetFrustum();
```

Le frustum sert a tester la visibilite d'un [[AABB]].

