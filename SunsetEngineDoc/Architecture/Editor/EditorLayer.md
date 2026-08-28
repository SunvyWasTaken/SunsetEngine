[[EditorLayer]] est dériver de [[Layer]].

[[EditorLayer]] possède un [[World]] dont il sera charger de mettre à jour et de render.

```cpp
void EditorLayer::Update(float deltatime)
{
	if (m_EditorState == EditorState::Play)
		m_World.Update(deltatime);
}

void EditorLayer::OnDraw()
{
	// renderScene.Begin(camera);
	// BuildRenderScene(m_World, renderScene);
	// renderScene.End();
}
```

Je crois que c'est lui qui doit possèder du coup un [[RenderGraph]].
et un [[RenderTarget]] qui sera le WorldRender.

```cpp
class EditorLayer
{
	RenderGraph renderGraph;
	RenderTarget renderTarget;
	RenderContext context;
	
	void Init()
	{
		RenderPass worldPass;
		worldPass.BindTarget(renderTarget);
		renderGraph.AddPass(worldPass);
	}
	
	void Update(float deltatime)
	{
		renderGraph(context);
		
		ImGui::Begin("Viewport");
		ImGui::Image(renderTarget.GetColorTexture());
		ImGui::End();
	}
}
```

Il va aussi possèder un [[RenderScene]] lui permetant de dessiner dans le [[RenderGraph]]->[[RenderTarget]].
