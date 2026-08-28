#Idea
## Concept

Le [[RenderGraph]] orchestre les [[RenderPass]] et leur dépendance.

```cpp
class RenderGraph final
{
public:
	void AddPass(const std::unique_ptr<RenderPass>& pass);
	
	void operator()(RenderContext& context);
private:
	std::vector<std::unique_ptr<RenderPass>> m_Passes;
};
```

Pour le moment je pense que je vais juste iterer à traver mes pass et les executer au lieu de faire un tri par rapport au dépendance de chacun.

```cpp
void RenderGraph::operator()(RenderContext& context)
{
	for (const auto& pass : m_Passes)
		pass(context);
}
```
## How to Use


