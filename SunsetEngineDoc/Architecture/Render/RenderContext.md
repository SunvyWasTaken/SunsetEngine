Le [[RenderContext]]...

```cpp
class RenderContext
{
public:
	void BindRenderTarget(RenderTarget target)
	{
		//...
	}
	
	void BindPipeline(Pipeline pipeline)
	{
		//...
	}
	
	void BindTexture(std::uint32_t slot, Texture texture)
	{
		//...
	}
	
	void Draw(const DrawCmd& cmds)
	{
		//...
	}
	
	void DrawFullscreen()
	{
		//...
	}
}
```
