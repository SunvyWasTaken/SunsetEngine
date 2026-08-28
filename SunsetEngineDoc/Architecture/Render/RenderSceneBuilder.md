#Idea #RenderScene
## Concept

Le [[RenderSceneBuilder]] sait quoi prendre du [World](obsidian://open?vault=SunsetEngineDoc&file=World) pour ensuite crée une liste d'[DrawCmd](obsidian://open?vault=SunsetEngineDoc&file=DrawCmd) et l'envoyé dans le [[RenderScene]].

```cpp
class RenderSceneBuilder
{
	void operator()(const World& world, RenderScene& scene)
	{
		world.each(
			[&](const Entity& entity,
				const TransformComponent& transform,
				const SpriteRenderComponent& Sprc)
		{
			scene.m_ObjectToRender.emplace_back(
				ObjectRenderItem{
					Sprc.m_drawable.m_Mesh,
					Sprc.m_drawable.m_Material,
					transform.GetWorldMatrix()
				}
			);
		});
	}
}
```