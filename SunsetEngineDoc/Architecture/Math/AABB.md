(Engine/Math/AABB.h)
## Concept

[[AABB]] est une bounding box alignee sur les axes.

```cpp
struct AABB
{
	glm::vec3 min;
	glm::vec3 max;
};
```

Elle sert aux tests spatiaux simples.

```cpp
box.contains(point);
box.intersects(otherBox);
```

[[Frustum]] peut aussi tester la visibilite d'une [[AABB]].

