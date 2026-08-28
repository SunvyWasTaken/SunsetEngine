#Idea
## Concept

Le [[RenderPass]] est une opération de rendu qui consomme des ressources et produit/modifie des ressources.

Il reçoit des données transmis depuis un [[RenderContext]] qu'il va transformer en suivant les étapes d'écrite dans `void operator()(RenderContext& context)`.

Le résultat est ensuite stocker dans un Attachment/Texture ou plusieurs *(BaseColor, DephTexture)*. Contenu par un [[RenderTarget]].

```cpp
class RenderPass
{
public:
	virtual ~RenderPass() = default;
	
	// décrit les étapes à réaliser.
	// C'est t'as recette.
	// écrit chaque étapes que ton render doit faire.
	virtual void operator()(RenderContext& context) = 0;
}
```
## Exemple

Exemple d'une Pass simple avec des objets à rendre.
```cpp
class BaseColorPass final : public RenderPass
{
	~BaseColorPass() override = default;
	
	void operator()(RenderContext& context) override
	{	
		context.BindRenderTarget(context.GetTarget("BaseColor"));
		
		context.SetPipeline(m_BaseColorPipeline);
		
		auto& baseColorQueue = context.GetQueue("BaseColor");
		
		context.Draw(baseColorQueue);
	}
	
	Pipeline m_BaseColorPipeline;
}
```

Exemple d'une Pass autre qu'avec des objets.
```cpp
class PostProcessPass final : public RenderPass
{
	~PostProcessPass() override = default;
	
	void operator()(RenderContext& context) override
	{
		auto input = context.Read("LastImage");
		
		// Bind le framebuffer de Final image
		// pour que le travail soit effecture dessus
		context.BindRenderTarget(context.GetTarget("FinalImage"));
		
		// Bind le shader en charge de la modification 
		context.BindPipeline(m_PostProcessPipeline);
		
		// Bind la texture à utilisé. I guess.
		context.BindTexture(0, input);
		
		context.DrawFullscreen();
	}
}
```

## How to Use

```cpp
{
	BaseColorPass baseColorPass;
	
	RenderContext context;
	
	baseColorPass(context);
}
```
