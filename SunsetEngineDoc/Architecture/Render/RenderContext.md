#Idea #Deprecated
## Concept

Le [[RenderContext]] est l'interface par laquelle le RenderPass passe pour effectuer des rendu. Afin de ne pas dépendre d'un context global issue de [RenderCommand](obsidian://open?vault=SunsetEngineDoc&file=RenderCommand).

Le Render Context possède la liste des [[DrawQueue]] réaliser je ne sais pas trop encore quand. pour que ensuite lui puisse avoir genre la queue BaseColor, Shadow, ect...

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
	
	DrawQueue GetQueue(const std::string_view& target)
	{
		return m_Queues.at(target);
	}
	
	void Draw(const DrawCmd& cmds)
	{
		//...
	}
	
	void DrawFullscreen()
	{
		//...
	}
private:
	std::unordered_map<std::string, DrawQueue> m_Queues;
}
```
