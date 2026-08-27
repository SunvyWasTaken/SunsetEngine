La [[DrawQueue]] est juste un objet qui va possède une liste de [[DrawCmd]]
```cpp
// .h
class DrawQueue
{
public:
	void AddDrawCmd(const DrawCmd& cmd);
	void Flush();
private:
	std::vector<DrawCmd> m_DrawCmds;
}
```

```cpp
// .cpp
void DrawQueue::AddDrawCmd(const DrawCmd& cmd)
{
	m_DrawCmds.emplace_back(cmd);
}

void DrawQueue::Flush()
{
	for (const auto& cmd : m_DrawCmds)
	{
		RenderAPI()->Draw(cmd);
	}
	m_DrawCmds.clear();
}
```
