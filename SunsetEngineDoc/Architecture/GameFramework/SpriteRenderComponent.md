(Engine/GameFramework/Components/SpriteRenderComponent.h)
## Concept

[[SpriteRenderComponent]] donne une representation dessinable a une [[Entity]].

Il possede un [[Drawable]].

```cpp
class SpriteRenderComponent : public Component
{
	Drawable m_drawable;
};
```

[[BuildRenderScene]] lit ce component avec le [[TransformComponent]]
pour remplir une [[RenderScene]].

