#Idea #RenderScene
## Concept

Le [[RenderScene]] possède une liste de [[DrawCmd]].

```cpp
class RenderScene
{
friend class RenderSceneBuilder;
public:
	void BeginScene(const Camera& camera)
	{
		m_Camera = camera;
	}
	
	const std::vector<DrawCmd>& GetObjects()
	{
		return m_ObjectToRender;
	}
	
private:
	std::vector<DrawCmd> m_ObjectToRender;
	Camera m_Camera;
}
```

## How to Use

```cpp
{
	//...
	renderScene.BeginScene(camera);
	renderSceneBuilder(world, renderScene);
	renderer.;
	//...
}
```
