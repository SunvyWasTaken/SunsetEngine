#Idea
## Concept

Le [[RenderTarget]] est la structure contenant le ColorAttachment et DepthAttachment.

```cpp
class RenderTarget
{
public:
	// Cet fonction est implementer ailleurs.
	// Le RenderAPI ce charge de crée la bonne implémentation.
	static RenderTarget* Create(const RenderTargetSpecification& specification);
public:
	virtual ~RenderTarget() = default;
	
	virtual void Bind() = 0;
	// Est ce que j'ai vrm besoin du Unbind si
	// le bind suivant override juste le précédent
	virtual void UnBind() = 0;
	
	virtual std::uint32_t ColorAttachment() const = 0;
	virtual std::uint32_t DepthAttachment() const = 0;
}
```

## OpenGL implémentation

```cpp
class OpenGLRenderTarget : public RenderTarget
{
public:
	OpenGLRenderTarget(uint32_t width, uint32_t height)
	{
		// ...
	}
	
	void Bind() override
	{
		// ...
	}
	
	void Unbind() override
	{
		// ...
	}
	
	uint32_t ColorAttachment() const override
	{
		return m_ColorTexture;
	}
	uint32_t DepthAttachment() const override
	{
		return m_DepthTexture;
	}
private:
	uint32_t m_Framebuffer = 0;
	uint32_t m_ColorAttachment = 0;
	uint32_t m_DepthAttachment = 0;
};
```

## How to Use

```cpp
auto worldTarget = RenderTarget::Create({
	.Width = 1920,
	.Height = 1080
});

worldTarget.Bind();
// Don't forget to clear the. render target otherwise it's gonna be black
renderer.Clear();
// ...
// do rendering here
// ...
worldTarget.UnBind();
```
