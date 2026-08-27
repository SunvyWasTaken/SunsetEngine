
Le [[RenderTarget]] est la structure contenant le ColorAttachment et DepthAttachment.

```cpp
class RenderTarget
{
public:
	virtual ~RenderTarget() = default;
	
	virtual void Bind() = 0;
	// Est ce que j'ai vrm besoin du Unbind si
	// le bind suivant override juste le précédent
	virtual void UnBind() = 0;
	
	virtual std::uint32_t GetColorTexture() const = 0;
	virtual std::uint32_t GetDepthTexture() const = 0;
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
	
	uint32_t GetColorTexture() const override
	{
		return m_ColorTexture;
	}
	uint32_t GetDepthTexture() const override
	{
		return m_DepthTexture;
	}
private:
	uint32_t m_Framebuffer = 0;
	uint32_t m_ColorTexture = 0;
	uint32_t m_DepthTexture = 0;
};
```
