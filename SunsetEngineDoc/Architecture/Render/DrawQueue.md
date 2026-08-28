#Idea #Deprecated
## Concept

La [[DrawQueue]] est juste un objet qui va possède une liste de [[DrawCmd]]

```cpp
class DrawQueue final
{
public:
	void AddDrawCmd(const DrawCmd& cmd)
	{
		m_DrawCmds.emplace_back(cmd);
	}
	
	void Flush()
	{
		for (const auto& cmd : m_DrawCmds)
		{
			RenderAPI()->Draw(cmd);
		}
		m_DrawCmds.clear();
	}
	
private:
	std::vector<DrawCmd> m_DrawCmds;
}
```