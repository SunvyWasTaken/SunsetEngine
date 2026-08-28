[[EditorLayer]] est dériver de [[Layer]].
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
