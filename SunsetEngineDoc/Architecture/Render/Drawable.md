(Engine/Render/Resources/Drawable.h)
## Concept

[[Drawable]] est la combinaison d'un [[Mesh]] et d'un [[Material]].

```cpp
class Drawable
{
public:
	std::shared_ptr<Mesh> m_Mesh;
	std::shared_ptr<Material> m_Material;
};
```

Un objet est drawable quand il possede les deux.

```cpp
if (drawable)
	RenderCommand::Submit(drawable, transform);
```

[[SpriteRenderComponent]] garde un [[Drawable]] pour etre ajoute dans une [[RenderScene]].

