(RenderAPI/OpenGL/Resources/OpenGLRenderTarget.h)
## Concept

[[OpenGLRenderTarget]] est l'implementation OpenGL de [[RenderTarget]].

Il possede un framebuffer, une color attachment et une depth attachment.

```cpp
std::uint32_t m_RenderID;
std::uint32_t m_ColorAttachment;
std::uint32_t m_DepthAttachment;
```

`Bind` rend dans le framebuffer.

`UnBind` revient au framebuffer par defaut.

