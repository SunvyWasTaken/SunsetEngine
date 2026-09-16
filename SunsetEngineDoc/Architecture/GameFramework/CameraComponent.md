(Engine/GameFramework/Components/CameraComponent.h)
## Concept

[[CameraComponent]] attache une [[Camera]] a une [[Entity]].

```cpp
class CameraComponent : public Component
{
public:
	bool Primary = false;
	Camera camera;
};
```

`Primary` indique quelle camera doit etre utilisee comme camera active par le rendu.

Le [[CameraSystem]] met a jour les cameras du [[World]].

